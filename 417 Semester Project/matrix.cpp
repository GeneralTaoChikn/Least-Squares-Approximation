#include "matrix.h"
//----------------------------Object-Constructors--------------------------------------
///Default Constructor
Matrix::Matrix()
{

}

///Constructor
Matrix::Matrix(istream& toread)
{
    this->readings = parse_raw_temps(toread);
    this->setX_Matrix();
    this->Y_matrices = setY_Matrix();
    this->XT_matrix = Transpose(X_matrix);
    this->setXTX_Matrix();
    this->setXTY_Matrix();
    this->setXTX_XTY_Matrix();
//    this->XTX_XTY_matrix = RowReduce(XTX_XTY_matrix);
    this->LinearInterpolate = PieceWiseLinearInterpolation();
}

///Destructor
Matrix::~Matrix()
{

}

//----------------------------Matrix-Constructors--------------------------------------
///*******Shit Works
void Matrix::setX_Matrix()
{
    for (const CoreTempReading& theReading : readings) {
        //Constructs X_ matrix
        MatrixContent toPush;
        toPush.push_back(1);
        toPush.push_back(theReading.first);
        X_matrix.push_back(toPush);
        toPush.clear();
    }//end for

}

///*******Shit Works
vector<vector<MatrixContent>> Matrix::setY_Matrix()
{

    vector<MatrixContent> Y_matrix;
    for (const CoreTempReading& theReading : readings) {
        //Construct Y_matrices
        Y_matrix.push_back(theReading.second);
    }//end for

    ///Y-matrices will split into for 4 vectors each with [12rows][1column]
    vector<vector<MatrixContent>> Parse_To_Y_vectors(Y_matrix[0].size(), vector<MatrixContent>(Y_matrix.size(), MatrixContent ()));

    for(int core = 0; core < Y_matrix[0].size(); core++){
        for(int row = 0; row < Y_matrix.size(); row++){
            Parse_To_Y_vectors[core][row].push_back(Y_matrix[row][core]);
        }//end for
    }//end for

    return Parse_To_Y_vectors;
}


///*******Shit Works
void Matrix::setXTX_Matrix()
{
    XTX_matrix = multiply(XT_matrix,X_matrix);
}


///*******Shit Works
void Matrix::setXTY_Matrix()
{
    for (const vector<MatrixContent>& yvector  : Y_matrices){
        XTY_matrices.push_back(multiply(XT_matrix,yvector));
    }//end for
}

void Matrix::setXTX_XTY_Matrix()
{
    for (const vector<MatrixContent>& XTY_matrix  : XTY_matrices){
        XTX_XTY_matrix.push_back(Augment(XTX_matrix, XTY_matrix));
    }
}

//----------------------------Matrix-Operators----------------------------------------------
///*******Shit Works
vector<MatrixContent>  Matrix::multiply(vector<MatrixContent> lhs, vector<MatrixContent> rhs)
{
    vector<MatrixContent> product(lhs.size(),MatrixContent(rhs[0].size()));

    for(int i = 0; i < lhs.size(); i++){
        for(int j = 0; j < rhs[0].size(); j++){

            product[i][j] = 0;

            for(int k = 0; k < rhs.size(); k++){
                product[i][j] += lhs[i][k] * rhs[k][j];
            }//end for
        }//end for
    }//end for

    return product;

}

///*******Shit Works
vector<MatrixContent> Matrix::Transpose(vector<MatrixContent> matrix)
{
     //Transpose  matrix
    vector<MatrixContent> transpose(matrix[0].size(),vector<double>());
    for(int i=0; i < matrix.size();i++){
        for(int j=0; j < matrix[0].size() ;j++){
            transpose[j].push_back(matrix[i][j]);
        }//end for
    }//end for

    return transpose;
}

///*******Shit Works
vector<MatrixContent> Matrix::Augment(vector<MatrixContent> lhs,
                                                vector<MatrixContent> rhs)
{

    vector<MatrixContent> AugMat(lhs.size(),MatrixContent());

    /**
     * y - indicates y col
     * i - indicates row
     * j - indicates x col
     */

    for (int y = 0; y < rhs[0].size(); y++) {
        for(int i = 0; i < lhs.size(); i++) {
            for (int j = 0; j < lhs[0].size(); j++){
                AugMat[i].push_back(lhs[i][j]);
            }
            AugMat[i].push_back(rhs[i][y]);
        }//end for
    }//end for


    return AugMat;

}

///*******Garbage
vector<vector<MatrixContent>> Matrix::RowReduce(vector<vector<MatrixContent>> matrix)
{
    const int nrows = matrix[0].size(); // number of rows
    const int ncols = matrix[0][0].size(); // number of columns
    int lead = 0;

    vector<vector<MatrixContent>> RowEchelonForm = matrix;


    // pass in matrix like rowecheon form core
    for(int core = 0; core < matrix.size(); core++){
            for (int r = 0; r < nrows; r++) { // for each row ...

                int idx = largestRowByCol(RowEchelonForm[core], 0,ncols);

                swapRows(RowEchelonForm[core], r, idx );

                scaleMatrix(RowEchelonForm[core], r, ncols, RowEchelonForm[core][r][r]);
                RowEchelonForm [core][r][r] = 1;

                eliminate(RowEchelonForm[core],r,ncols,nrows);

        }//end for
    }//end while

    return RowEchelonForm;

}

///*******Shit Works
vector<vector<MatrixContent>> Matrix::PieceWiseLinearInterpolation()
{

    vector<vector<MatrixContent>> interpolate(Y_matrices.size(),
                                              vector<MatrixContent>(X_matrix.size()-1,
                                                                     MatrixContent ()));

    double x0, x1, y0, y1, c0, c1;
    for (int core = 0; core < Y_matrices.size(); core++) {
        for(int row = 0; row < (X_matrix.size()-1); row++){

            x0 = X_matrix[row][1];
            x1 = X_matrix[row+1][1];
            y0 = Y_matrices[core][row][0];
            y1 = Y_matrices[core][row+1][0];
            c1 = ((y1-y0)/(x1-x0));
            c0 = y0 - (c1 * x0);

            interpolate[core][row].push_back(x0);
            interpolate[core][row].push_back(x1);
            interpolate[core][row].push_back(c0);
            interpolate[core][row].push_back(c1);


        }//end for
    }//end for

    return interpolate;

}
//----------------------------Matrix-Read-SHIT-----------------------------------------
vector<CoreTempReading> Matrix::parse_raw_temps(std::istream& original_temps,
                                             int step_size)
{
    std::vector<CoreTempReading> allTheReadings;

    // Input Parsing Variables
    int step = 0;
    std::string line;

    while (getline(original_temps, line)) {
        std::istringstream input(line);

        std::vector<double> next_temperature_set;
        std::transform(std::istream_iterator<std::string>(input),
                       std::istream_iterator<std::string>(),
                       std::back_inserter(next_temperature_set),
                       [](const std::string& raw_reading) -> double {
                           return stod(raw_reading);
                       });

        allTheReadings.emplace_back(step, next_temperature_set);
        step += step_size;
    }

    return allTheReadings;
}
//----------------------------Matrix-Display-SHIT-------------------------------------
void Matrix::display()
{
//        for (const CoreTempReading& theReading : readings) {
//        cout << "(" << theReading.first << ", [";
////        fout << "(" << theReading.first << ", [";
//
//        const vector<double>& coreTemps = theReading.second;
//        for (int i = 0; i < coreTemps.size() -1 ; i++) {
//            cout << coreTemps[i] << ", ";
////            fout << coreTemps[i] << ", ";
//        }
//        cout << *(coreTemps.end() - 1) << "])" << "\n";
////        fout << *(coreTemps.end() - 1) << "])" << "\n";
//    }
//
////    for(double x : Y_matrix)
////        cout << '\n' << x <<'\n';

//    string text;
//    for (int core = 0; core < XTX_XTY_matrix.size(); core ++){
//        text = ""
//    }
    ///Tests the data structures
    ofstream fout;
    fout.open("output1.txt");

        ///Display X
    cout << '\n' << "Display X" << '\n';
    fout << '\n' << "Display X" << '\n';
    for (const MatrixContent& x: X_matrix){
        for (double xc:x){
            cout << " " << xc;
            fout << " " << xc;
        }
        cout << '\n';
        fout << '\n';
    }

        ///Display Y vectors    Core
    cout << '\n' << "Display Y vectors" << '\n';
    fout << '\n' << "Display Y vectors" << '\n';
        int i =1;
    for (const vector<MatrixContent>& core: Y_matrices){
        cout <<"VECTOR " << i << '\n'<< '\n';
        fout <<"VECTOR " << i << '\n'<< '\n';
        for(const MatrixContent& row: core){
            for (const double& xtc:row){
                cout << " " << xtc ;
                fout << " " << xtc ;
            }
            cout << '\n';
            fout << '\n';
        }
        cout << '\n';
        fout << '\n';
        i++;
    }

        ///Display XT
    cout << '\n' << "Display XT" << '\n';
    fout << '\n' << "Display XT" << '\n';
    for (const MatrixContent& xt: XT_matrix){
        for (double xtc:xt){
            cout << " " << xtc ;
            fout << " " << xtc ;
        }
         cout << '\n';
         fout << '\n';
    }

        ///Display XTX
        cout << '\n' << "Display XTX" << '\n';
        fout << '\n' << "Display XTX" << '\n';
    for (const MatrixContent& xt: XTX_matrix){
        for (double xtc:xt){
            cout << " " << xtc ;
            fout << " " << xtc ;
        }
         cout << '\n';
         fout << '\n';
    }

        ///Display XTY      core
    cout << '\n' << "Display XTY" << '\n';
    fout << '\n' << "Display XTY" << '\n';
    for (const vector<MatrixContent>& xty1d: XTY_matrices){
        for(const MatrixContent& xty2d: xty1d){
            for (double xtc:xty2d){
                cout << " " << xtc ;
                fout << " " << xtc ;
            }
            cout << '\n';
            fout << '\n';
        }
        cout << '\n';
        fout << '\n';
    }

        ///Display XTX XTY    core
    cout << '\n' << "Display XTX|XTY" << '\n';
    fout << '\n' << "Display XTX|XTY" << '\n';
    for (const vector<MatrixContent>& xty1d: XTX_XTY_matrix){
        for(const MatrixContent& xty2d: xty1d){
            for (double xtc:xty2d){
                cout << " " << xtc ;
                fout << " " << xtc ;
            }
            cout << '\n';
            fout << '\n';
        }
        cout << '\n';
        fout << '\n';
    }


            ///Interpolate    core
    cout << '\n' << "Interpolate" << '\n';
    fout << '\n' << "Interpolate" << '\n';
//    cout << LinearInterpolate[1][1][0];

    for (int core = 0; core < numberOfCores()-1; core++){
        for(int row = 0; row < LinearInterpolate[0].size(); row++){

//    for (int core = 0; core < ; core++){
//        for(int row = 0; row < ; row++){
            double x0, x1, c0, c1;
            x0 = LinearInterpolate[core][row][0];
            x1 = LinearInterpolate[core][row][1];
            c0 = LinearInterpolate[core][row][2];
            c1 = LinearInterpolate[core][row][3];

                cout << x0 << " <= x < " << x1 << "; y = " << c0 << " + " << c1 << "x ; Interpolate";
//                cout << LinearInterpolate[core][row][0] << " <= x <" << LinearInterpolate[core][row][1] << "; y = " << LinearInterpolate[core][row][2] << " + " << LinearInterpolate[core][row][3] << "x ; Interpolate";
            cout << '\n';
            fout << '\n';
        }
        cout << '\n';
        fout << '\n';
    }

//    for (const vector<MatrixContent>& xty1d: LinearInterpolate){
//        for(const MatrixContent& xty2d: xty1d){
//            for (double xtc:xty2d){
//                cout << " " << xtc ;
//                fout << " " << xtc ;
//            }
//            cout << '\n';
//            fout << '\n';
//        }
//        cout << '\n';
//        fout << '\n';
//    }

    fout.close();

}

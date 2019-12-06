#include <stdio.h>
#include "matrix.h"

//----------------------------Object-Constructors--------------------------------------
//Default Constructor
Matrix::Matrix()
{

}

//Constructor
Matrix::Matrix(istream& toread)
{
    this->readings = parse_raw_temps(toread);
    this->setX_Matrix();
    this->Y_matrices = setY_Matrix();
    this->XT_matrix = Transpose(X_matrix);
    this->setXTX_Matrix();
    this->setXTY_Matrix();
    this->setXTX_XTY_Matrix();
    this->RowReduce(XTX_XTY_matrix);
    this->LinearInterpolate = PieceWiseLinearInterpolation();
}

//Destructor
Matrix::~Matrix()
{
    readings.clear();

}

//----------------------------Matrix-Constructors--------------------------------------

void Matrix::setX_Matrix()
{
    //Read in Time
    for (const CoreTempReading& theReading : readings) {
        //Constructs X_ matrix
        MatrixContent toPush;
        toPush.push_back(1);
        toPush.push_back(theReading.first);
        X_matrix.push_back(toPush);
        toPush.clear();
    }//end for

}

vector<vector<MatrixContent>> Matrix::setY_Matrix()
{
    //Read in temperatures
    vector<MatrixContent> Y_matrix;
    for (const CoreTempReading& theReading : readings) {
        //Construct Y_matrices
        Y_matrix.push_back(theReading.second);
    }//end for

    //Sets Dimension so it doesn't go out of scope
    vector<vector<MatrixContent>> Parse_To_Y_vectors(Y_matrix[0].size(),
                                                     vector<MatrixContent>(Y_matrix.size(),
                                                                           MatrixContent ()));
    //Break up Matrix into single vectors
    for(int core = 0; core < Y_matrix[0].size(); core++){
        for(int row = 0; row < Y_matrix.size(); row++){
            Parse_To_Y_vectors[core][row].push_back(Y_matrix[row][core]);
        }//end for
    }//end for

    return Parse_To_Y_vectors;
}

void Matrix::setXTX_Matrix()
{
    XTX_matrix = multiply(XT_matrix,X_matrix);
}

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


vector<MatrixContent> Matrix::Transpose(vector<MatrixContent> matrix)
{
     //Transpose  matrix
    vector<MatrixContent> transpose(matrix[0].size(),vector<double>());

    for(int row=0; row < matrix.size(); row++){
        for(int col=0; col < matrix[0].size() ;col++){
            transpose[col].push_back(matrix[row][col]);
        }//end for
    }//end for

    return transpose;
}


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

/**
 * These functions are modified versions.
 * Original Code is from this website
 * https://rosettacode.org/wiki/Reduced_row_echelon_form
 *
 * Thank You Rosetta Code! I couldn't get this crap
 * working without it!
 */
void Matrix::RowReduce(vector<vector<MatrixContent>>& matrix)
{
    const int nrows = matrix[0].size(); // number of rows
    const int ncols = matrix[0][0].size(); // number of columns
    int lead; // used to pivot

    for (vector<MatrixContent>& traverse : matrix){
        lead = 0;
        for (int row = 0; row < nrows; row++) { // for each row ...

            if (ncols <= lead)
                return;

            //Index
            int i = row;

            while (traverse[i][lead] == 0) {
                i++;

                if (nrows < i) {
                    i = row;
                    lead++;

                    if (ncols <= lead)
                        return;
                }//end if
            }//end while

            swapRows(traverse, i, row);

            divide_rows(traverse, row, traverse[row][lead]);


            for (i = 0; i < nrows; i++) {
                if (i != row)
                    add_multiple_row(traverse, i, row, -(traverse[i][lead]));
            }//end for
            lead++;
        }//end for
    }//end for
}


vector<vector<MatrixContent>> Matrix::PieceWiseLinearInterpolation()
{

    vector<vector<MatrixContent>> interpolate(Y_matrices.size(),
                                              vector<MatrixContent>(X_matrix.size()-1,
                                                                     MatrixContent ()));
    //Variables to Perfom Calculations
    double x0, x1, y0, y1, c0, c1;

    for (int core = 0; core < Y_matrices.size(); core++) {
        for(int row = 0; row < (X_matrix.size()-1); row++){

            x0 = X_matrix[row][1];
            x1 = X_matrix[row+1][1];
            y0 = Y_matrices[core][row][0];
            y1 = Y_matrices[core][row+1][0];
            c1 = ((y1-y0)/(x1-x0));		//slope
            c0 = y0 - (c1 * x0);		//Y-intercept

            interpolate[core][row].push_back(x0);
            interpolate[core][row].push_back(x1);
            interpolate[core][row].push_back(c0);
            interpolate[core][row].push_back(c1);


        }//end for
    }//end for

    return interpolate;

}
//----------------------------Matrix-Read-Crap-----------------------------------------
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
//----------------------------Matrix-Display-Crap-------------------------------------
void Matrix::display(string& File)
{
    ofstream fout;

    //Variables to output in format
    double x0, x1, c0, c1;

    string BaseName = File;
    BaseName.erase(BaseName.find_last_of('.'));

    for(int core = 0; core < numberOfCores(); core++){

        string Output = BaseName + "-core-" +
           to_string(core) + ".txt";

        fout.open(Output);




        ///Interpolate    core
        cout << "PieceWise Linear Interpolation" << '\n';
        fout << "PieceWise Linear Interpolation" << '\n';


        for(int row = 0; row < LinearInterpolate[0].size(); row++){

            x0 = LinearInterpolate[core][row][0];
            x1 = LinearInterpolate[core][row][1];
            c0 = LinearInterpolate[core][row][2];
            c1 = LinearInterpolate[core][row][3];

            cout << x0 << " <= x < " << x1 << "; y = "
                 << c0 << " + " << c1 << "x ; Interpolate";

            fout << x0 << " <= x < " << x1 << "; y = " << c0
                 << " + " << c1 << "x ; Interpolate";

            cout << '\n';
            fout << '\n';
        }//end for


        cout << '\n' <<"Global Least Squares Approximation" << '\n';
        fout << '\n' <<"Global Least Squares Approximation" << '\n';

        x0 = 0;
        x1 = X_matrix[getXsize()-1][1];
        c0 = XTX_XTY_matrix[core][0][2];
        c1 = XTX_XTY_matrix[core][1][2];

        cout << x0 << " <= x < " << x1 << "; y = "
            << c0 << " + " << c1 << "x ; Least-Squares" << '\n';

        fout << x0 << " <= x < " << x1 << "; y = " << c0
            << " + " << c1 << "x ; Least-Squares";

        fout.close();
    }//end for

}

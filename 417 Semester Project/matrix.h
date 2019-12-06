#include <algorithm>
#include <iomanip>
#include <iterator>
#include <list>
#include <vector>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <utility>



using namespace std;

/**
 * A pair of values where the
 *   - _first_ attribute represents the time at which the reading was taken
 *   - _second is a vector with _n_ temperature readings, where _n_ is the
 *     number of CPU Cores
 */
using CoreTempReading = std::pair<int, std::vector<double>>;

using MatrixContent = vector <double>;



/**
 * This class represents a matrix
 * This includes augmented, XTX, XTY matrices
 *
 * All matrices are stored in a vector of vectors containing doubles.
 *
 */
class Matrix{

private:

    ///The read in values
    vector<CoreTempReading> readings;

        //X matrix
    vector<MatrixContent> X_matrix;

        //Y matrix
    vector<vector<MatrixContent>> Y_matrices;

        //XT matrix
    vector<MatrixContent> XT_matrix;

        //XTY matrices
    vector<vector<MatrixContent>> XTY_matrices;

    vector<MatrixContent> XTX_matrix;

    vector<vector<MatrixContent>> XTX_XTY_matrix;

    vector<vector<MatrixContent>> LinearInterpolate;



public:
    /**
     * Default matrix to an empty object
     */
    Matrix();

    /**
     * Constructor
     */
    Matrix(istream& toread);

    /**
     * Destructor
     */
    virtual ~Matrix();

//------------------------------------------------------------------------------

    /**
     * Retrieve the matrix
     */
    vector<MatrixContent> getXMatrix;

    /**
     * Retrieve Y Matrices
     */
    vector<vector<MatrixContent>> getYMatrices();

    int numberOfCores() const;

    int getXsize() const;

    int getYsize() const;



//----------------------------Matrix-Makers--------------------------------------

    void setX_Matrix();

    vector<vector<MatrixContent>> setY_Matrix();

    void setXTX_Matrix();

    void setXTY_Matrix();

    void setXTX_XTY_Matrix();

//------------------------------------------------------------------------------

    /**
     * Read in the Times and temperatures of each core from a source file
     */
    vector<CoreTempReading> parse_raw_temps(std::istream& original_temps,
                                             int step_size = 30);
    /**
     * Multiply Two Matrices
     */
    vector<MatrixContent> multiply (vector<MatrixContent> lhs,
                                    vector<MatrixContent> rhs);

    vector<MatrixContent> Transpose (vector<MatrixContent> matrix);

    vector<vector<MatrixContent>> RowReduce(vector<vector<MatrixContent>> );

    vector<MatrixContent> Augment(vector<MatrixContent> lhs,
                                    vector<MatrixContent> rhs);

    vector<vector<MatrixContent>> PieceWiseLinearInterpolation();

    int largestRowByCol (vector<MatrixContent> , int , int);
    void swapRows (vector<MatrixContent>&, int, int);
    void scaleMatrix(vector<MatrixContent>&, int row_index, int num_cols, int s);
    void eliminate(vector<MatrixContent>&, int SRCrow_index, int num_cols, int num_rows);
    void backsolve(vector<MatrixContent>&);



    /**
     * Display the Readings
     */
    void display();

//    virtual void display(ostream& outs) const;
//
//    virtual void read(istream& ins);
//
//    virtual Matrix* clone() const;

};

//------------------------------------------------------------------------------
inline
int Matrix::numberOfCores() const
{
    return this->XTX_XTY_matrix.size();
}

inline
int Matrix::getXsize() const
{
    return this->X_matrix.size();
}

inline
int Matrix::getYsize() const
{
    return this->Y_matrices[0].size();
}


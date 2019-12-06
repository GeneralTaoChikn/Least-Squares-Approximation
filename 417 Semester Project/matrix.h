#include <algorithm>
#include <iomanip>
#include <iterator>
#include <list>
#include <vector>
#include <cstdio>
#include <string>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>
#include <utility>

#include <cassert>



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


    vector<CoreTempReading> readings; 				///< Storage for Time & Temperatures


    vector<MatrixContent> X_matrix; 				///< X matrix containing the times


    vector<vector<MatrixContent>> Y_matrices;		///< Container of Cores' Temperatures


    vector<MatrixContent> XT_matrix;				///< XT matrix


    vector<vector<MatrixContent>> XTY_matrices;		///< multiple XTY matrix


    vector<MatrixContent> XTX_matrix;				///< XTX matrix


    vector<vector<MatrixContent>> XTX_XTY_matrix;	///< Augmented XTX|XTY matrix


    vector<vector<MatrixContent>> LinearInterpolate;///< Interpolation readings



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

//---------------------------------Getters----------------------------------------

    /**
     * Retrieve the Number of cores
     */
    int numberOfCores() const;

    /**
     * Get the number row size of X matrix
     */
    int getXsize() const;

    /**
     * Get the number row size of Y matrix
     */
    int getYsize() const;



//----------------------------Matrix-Makers--------------------------------------
    /**
     * Sets the X matrix using CoreTemp Readings
     */
    void setX_Matrix();

    /**
     * Sets the Y matrix using CoreTemp Readings
     */
    vector<vector<MatrixContent>> setY_Matrix();

    /**
     * Sets the XTX through multiplication
     */
    void setXTX_Matrix();

    /**
     * Sets the XTY through multiplication
     */
    void setXTY_Matrix();

    /**
     * Augments to XTX|XTY
     */
    void setXTX_XTY_Matrix();

//------------------------------------------------------------------------------

    /**
     * Read in the Times and temperatures of each core from a source file
     * @param original_temps - file to read in
     * @param step_size - increment of time in seconds
     */
    vector<CoreTempReading> parse_raw_temps(std::istream& original_temps,
                                             int step_size = 30);
    /**
     * Multiply Two Matrices
     * @param lhs - matrix to multiply
     * @param rhs - matrix to multiply
     */
    vector<MatrixContent> multiply (vector<MatrixContent> lhs,
                                    vector<MatrixContent> rhs);

    /**
     * Transposes a Matrix
     * @param matrix - matrix to transpose
     */
    vector<MatrixContent> Transpose (vector<MatrixContent> matrix);

    /**
     * Augments a Matrix
     * @param lhs -  matrix to augment
     * @param rhs - matrix to augment
     */
    vector<MatrixContent> Augment(vector<MatrixContent> lhs,
                                    vector<MatrixContent> rhs);

    /**
     * Reduces to Echelon form
     * @param matrix to reduce to echelon form
     */
//    vector<vector<MatrixContent>> RowReduce(vector<vector<MatrixContent>> );
    void RowReduce(vector<vector<MatrixContent>>& );


    /**
     * Performs a Linear Interpolation
     */
    vector<vector<MatrixContent>> PieceWiseLinearInterpolation();

    /**
     * Functions used to Reduce to Echelon Form
     */

    void swapRows (vector<MatrixContent>&, int, int);
    void divide_rows (vector<MatrixContent>& , int i, int v);
    void add_multiple_row (vector<MatrixContent>& , int i, int k, int v);



    /**
     * Display the Readings
     */
    void display(string& );

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


#include "matrix.h"

/**
 * These functions are modified versions.
 * Original Code is from this website
 * https://rosettacode.org/wiki/Reduced_row_echelon_form
 *
 * Thank You Rosetta Code! I couldn't get this crap
 * working without it!
 */
void Matrix::swapRows(vector<MatrixContent>& matrix, int row, int pivot)
{
    for(int col = 0; col < matrix[0].size(); col++){
        swap(matrix[row][col], matrix[pivot][col]);
    }
}

void Matrix::divide_rows (vector<MatrixContent>& matrix, int i, int v)
{
    assert(v != 0);

    for (int col = 0; col < matrix[0].size(); col++) {
        matrix[i][col] /= v;
    }
}

void Matrix::add_multiple_row (vector<MatrixContent>& matrix, int i, int k, int v)
{
    for (int col = 0; col < matrix[0].size(); col++) {
        matrix[i][col] += v * (matrix [k][col]);
    }
}


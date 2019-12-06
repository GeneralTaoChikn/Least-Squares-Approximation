#include "matrix.h"


int Matrix::largestRowByCol(vector<MatrixContent> Matrix, int col_index, int num_rows)
{
    return 0;
}


void Matrix::scaleMatrix(vector<MatrixContent>& matrix, int row_index, int num_cols, int s)
{
    for (int j = 0; j < num_cols; j++){
        matrix[row_index][j] = matrix[row_index][j] / s;
    }
}

void Matrix::eliminate(vector<MatrixContent>& matrix, int SRCrow_index, int num_cols, int num_rows)
{
    int start_col = SRCrow_index;

    for (int i = start_col + 1; i < num_rows; i++){
        int s = matrix[i][start_col];

        for (int j = start_col; j < num_cols; j++){
            matrix[i][j] = matrix[i][j] - s * matrix[SRCrow_index][j];
        }//end for

        matrix[i][start_col] = 0;

    }//end for

}

void Matrix::backsolve(vector<MatrixContent>& matrix)
{
    int augColIdx = matrix[0].size();
    int lastrow = matrix.size()-1;

    for (int i = lastrow; i > 1; i--){
        for(int j = (i-1); i>0; j--){
            int s = matrix[j][i];

            matrix[j][i] -= (s * matrix[i][i]);
            matrix[j][augColIdx] -= (s * matrix[i][augColIdx]);
        }
    }

}

void Matrix::swapRows(vector<MatrixContent>& matrix, int row, int pivot)
{
    matrix[row].swap(matrix[pivot]);
}


//vector<vector<MatrixContent>> Matrix::RowReduce(vector<vector<MatrixContent>> matrix)
//{
//    const int nrows = matrix[0].size(); // number of rows
//    const int ncols = matrix[0][0].size(); // number of columns
//
//    vector<vector<MatrixContent>> RowEchelonForm = matrix;
//
//
//    for(int core = 0; core < matrix.size(); core++){
//        int lead = 0;
//        while (lead < nrows) {
//        float d, m;
//
//            for (int r = 0; r < nrows; r++) { // for each row ...
//                /* calculate divisor and multiplier */
//                d = RowEchelonForm[core][lead][lead];
//                m = RowEchelonForm[core][r][lead] / RowEchelonForm[core][lead][lead];
//
//                for (int c = 0; c < ncols; c++) { // for each column ...
//                    if (r == lead)
//                        RowEchelonForm[core][r][c] /= d;               // make pivot = 1
//                    else
//                        RowEchelonForm[core][r][c] -= RowEchelonForm[core][lead][c] * m;  // make other = 0
//                }//end for
//            }//end for
//
//        lead++;
//
//        }//end for
//    }//end while
//
//    return RowEchelonForm;
//
//}

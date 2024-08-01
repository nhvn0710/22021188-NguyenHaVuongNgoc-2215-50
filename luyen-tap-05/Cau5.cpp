int** transpose(int** matrix, int nRows, int nCols)
{
    int** A = new int* [nCols];
    for (int i = 0; i < nCols; i++) {
        A[i] = new int[nRows];
    }
    for (int i = 0; i < nCols; i++)
    {
        for (int j = 0; j < nRows; j++)
        {
            A[i][j] = matrix[j][i];
        }
    }
    return A;
}
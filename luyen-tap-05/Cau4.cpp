int** inputMatrix(int nRows, int nCols)
{
    int** matrix = new int* [nRows];
    for (int i = 0; i < nRows; i++) {
        matrix[i] = new int[nCols];
    }
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            int n0;
            cin >> n0;
            matrix[i][j] = n0;
        }
    }
    return matrix;
}

void printMatrix(int** matrix, int nRows, int nCols)
{
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            cout << matrix[i][j] << " ";
        }
        if (i < nRows - 1) cout << endl;
    }
}
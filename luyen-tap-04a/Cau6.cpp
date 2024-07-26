int** transpose(int** matrix, int nRows, int nCols)
{
	int** nuM = new int* [nCols];
	for (int i = 0; i < nCols; i++) {
		nuM[i] = new int[nRows];
	}
	for (int i = 0; i < nCols; i++)
	{
		for (int j = 0; j < nRows; j++)
		{
			nuM[i][j] = matrix[j][i];
		}
	}
	return nuM;
}
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int countMines(const vector<vector<char>>& M, int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int y = row + i;
            int x = col + j;
            int ny = M.size(), nx = M[0].size();
            if (y >= 0 && y < ny && x >= 0 && x < nx) {
                if (M[y][x] == '*') count++;
            }
        }
    }
    return count;
}

void mineSweeping()
{
    int m, n;
    cin >> m >> n;
    vector<vector<char>> A(m, vector<char>(n));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] == '*') {
                cout << "* ";
            }
            else {
                int count = countMines(A, i, j);
                cout << count << " ";
            }
        }
        cout << endl;
    }
}

int main() {

    mineSweeping();
    return 0;
}
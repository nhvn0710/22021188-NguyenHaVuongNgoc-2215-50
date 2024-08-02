#include <iostream>
#include <vector>

using namespace std;

int countMines(const vector<vector<char>>& M, int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int row0 = y + i;
            int col0 = x + j;
            int ny = M.size(), nx = M[0].size();
            if (row0 >= 0 && row0 < ny && col0 >= 0 && col0 < nx) {
                if (M[row0][col0] == '*') count++;
            }
        }
    }
    return count;
}

void printA(const vector<vector<char>>& M) {
    int ny = M.size(), nx = M[0].size();
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            cout << M[i][j];
        }
        cout << endl;
    }
}

void printB(const vector<vector<int>>& M) {
    int ny = M.size(), nx = M[0].size();
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j < nx; j++) {
            if (M[i][j] > -1) cout << " ";
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

void spreadtoCells(vector<vector<char>>& A, vector<vector<int>>& B, int y, int x) {
    int m = A.size();
    int n = A[0].size();

    if (y < 0 || y >= m || x < 0 || x >= n) {
        return;
    }

    if (B[y][x] > -1) return;

    int count = countMines(A, y, x);
    B[y][x] = count;

    if (count == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i == 0 && j == 0)) continue;
                spreadtoCells(A, B, y + i, x + j);
            }
        }
    }
}

void notmineSweeper()
{
    int m, n, k;
    cin >> m >> n >> k;

    vector<vector<char>> A(m, vector<char>(n));
    vector<vector<int>> B(m, vector<int>(n, -1));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    while (true) {
        int y, x;
        cin >> y >> x;

        if (y < 1 || y > m || x < 1 || x > n) {
            continue;
        }

        y--; x--;

        if (A[y][x] == 'M') {
            cout << "YOU'RE DEAD!" << endl;
            printA(A);
            break;
        }
        else {
            int count = countMines(A, y, x);
            B[y][x] = count;
            if (count == 0) {
                spreadtoCells(A, B, y, x);
            }
            printB(B);
        }
    }
}

int main() {
    notmineSweeper();
    return 0;
}
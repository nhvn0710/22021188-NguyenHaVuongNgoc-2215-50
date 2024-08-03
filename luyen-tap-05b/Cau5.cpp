#include <iostream>
#include <vector>

using namespace std;

void printPascalTriangle(int n) {
    vector<vector<int>> A(n);

    for (int i = 0; i < n; i++) {
        A[i].resize(i + 1);
        A[i][0] = A[i][i] = 1;

        for (int j = 1; j < i; j++) {
            A[i][j] = A[i - 1][j - 1] + A[i - 1][j];
        }
    }
    int ny = A.size(), nx;
    for (int i = 0; i < ny; i++) {
        nx = A[i].size();
        for (int j = 0; j < nx; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;
    cin >> n;
    printPascalTriangle(n + 1);
    return 0;
}
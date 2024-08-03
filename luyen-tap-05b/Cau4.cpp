#include <iostream>
#include <vector>
using namespace std;

void siameseMagicSquare(int n)
{
    vector<vector<int>> A(n + 2, vector<int>(n + 2, 0));
    int x = n / 2, y = n - 1;
    for (int i = 1; i < n * n + 1; i++)
    {
        int tmpx = x, tmpy = y;
        x++; y++;
        if (x > n) x = x % n;
        if (y > n) y = y % n;
        if (A[x][y] != 0)
        {
            x = tmpx; y = tmpy;
            y--;
        }
        A[x][y] = i;
    }
    for (int j = n; j > 0; j--)
    {
        for (int i = 1; i < n + 1; i++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;
    cin >> n;
    siameseMagicSquare(n);

    return 0;
}
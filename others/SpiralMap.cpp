#include <iostream>
#include <vector>

using namespace std;

void printSpiralMap(const int m, const int n)
{
    vector<vector<int>> A(m, vector<int>(n, 0));

    vector<int> dy = { 0, 1, 0, -1 };  //1=u,2=r,3=d,4=l
    vector<int> dx = { 1, 0, -1, 0 };

    int y = 0, x = 0, dir = 0;

    for (int i = 1; i < m * n + 1; i++)
    {
        A[y][x] = i;
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (nextY >= m || nextX >= n || nextY < 0 || nextX < 0)
        {
            dir = (dir + 1) % 4;
            nextY = y + dy[dir];
            nextX = x + dx[dir];
        }
        else
        {
            if (A[nextY][nextX] != 0)
            {
                dir = (dir + 1) % 4;
                nextY = y + dy[dir];
                nextX = x + dx[dir];
            }
        }
        y = nextY;
        x = nextX;
    }

    for (int im = 0; im < m; im++)
    {
        for (int in = 0; in < n; in++)
        {
            cout << A[im][in] << " ";
        }
        cout << endl;
    }
}

int main() {
    int m, n;
    cin >> m >> n;
    printSpiralMap(m, n);
}

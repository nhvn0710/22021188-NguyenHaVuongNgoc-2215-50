#include <iostream>
#include <vector>

using namespace std;

void printPrimeFactors(int num)
{
    if (num < 2)
    {
        return;
    }
    if (num < 4)
    {
        cout << num << " " << 1 << endl;
        return;
    }

    vector<vector<int>> A(2, vector<int>());
    int cnt = 0;
    while (num % 2 == 0)
    {
        cnt++;
        num = num / 2;
    }
    if (cnt > 0)
    {
        A[0].push_back(2);
        A[1].push_back(cnt);
    }

    for (int i = 3; i <= num / 2 || num > 1; i += 2) {
        cnt = 0;
        while (num % i == 0)
        {
            cnt++;
            num = num / i;
        }
        if (cnt > 0)
        {
            A[0].push_back(i);
            A[1].push_back(cnt);
        }
    }
    int l = A[0].size();
    for (int i = 0; i < l; i++)
    {
        cout << A[0][i] << " " << A[1][i] << endl;
    }
}

int main() {
    int n;
    cin >> n;
    printPrimeFactors(n);
    return 0;
}
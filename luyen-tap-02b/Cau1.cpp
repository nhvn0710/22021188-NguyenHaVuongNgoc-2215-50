#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void sumFancy(int n)
{
    long outp = 0;
    vector<int> A = {};
    for (int i = 0; i < n; i++)
    {
        int tmp;
        cin >> tmp;
        A.push_back(tmp);
    }
    sort(A.begin(), A.end());
    for (int i = 0; i < n; i++)
    {
        outp = outp + A[i] * (n - i - 1);
    }

    cout << outp;
}

int main() {
    int n;
    cin >> n;
    sumFancy(n);

    return 0;
}
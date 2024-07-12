#include <iostream>
#include <vector>

using namespace std;

int numofDivisor(int n)
{
    if (n > 0) { n = -n; }
    if (n == 0) { return -1; }
    int n0 = n, count = 0;
    while (n != 0)
    {
        int tmp = n % 10;
        if (tmp != 0) {
            if (n0 % tmp == 0)
            {
                count++;
            }
        }
        n = n / 10;
    }
    return count;
}

int main() {
    int n;
    cin >> n;

    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    for (int i = 0; i < n; i++) {
        int t = numofDivisor(A[i]);
        cout << t;
        if (i != n - 1) { cout << endl; }
    }


    return 0;
}

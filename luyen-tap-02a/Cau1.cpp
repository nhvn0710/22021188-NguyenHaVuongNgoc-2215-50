#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    sort(A.begin(), A.end());

    int max = 1;
    int count = 1;
    int start = 0;

    for (int i = 1; i < n; i++) {
        if (A[i] - A[start] > 1) {
            if (count > max) { max = count; }
            start = start + count;
            count = 0;
        }
        count++;
    }

    if (count > max) { max = count; }

    cout << max;

    return 0;
}
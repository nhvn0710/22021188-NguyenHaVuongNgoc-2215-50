#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < i + n; j++)
        {
            cout << j % n + 1;
            if (j != i + n - 1) { cout << " "; }
        }
        if (i != n - 1) { cout << endl; }
    }

    return 0;
}
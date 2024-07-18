#include <iostream>

using namespace std;

int main() {
    int n, x, y, a = 1000000, b = 1000000;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        if (x < a) a = x;
        if (y < b) b = y;
    }
    cout << a * b;
    return 0;
}

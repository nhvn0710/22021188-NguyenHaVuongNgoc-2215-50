#include <iostream>

using namespace std;

int fibonacci(int n) {
    if (n < 1) {
        return 0;
    }
    if (n < 2) {
        return 1;
    }
    int n0 = 0, n1 = 1, n2;
    for (int i = 1; i < n; i++) {
        n2 = n0 + n1;
        n0 = n1;
        n1 = n2;
    }
    return n2;
    // Complete the function.

}

int main() {
    int n;
    cin >> n;
    cout << fibonacci(n);
    return 0;
}

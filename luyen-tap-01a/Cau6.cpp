#include <iostream>
#include <cmath>

using namespace std;

void isSquare(int a) {
    int d = sqrt(a);
    if (d * d == a) { cout << "YES"; }
    else { cout << "NO"; }
}

int main() {
    int a;
    cin >> a;
    isSquare(a);

    return 0;
}
#include <iostream>

using namespace std;

int main() {
    int a, b, c, a2, b2, c2;
    cin >> a >> b >> c >> a2 >> b2 >> c2;
    int d = a * b2 - a2 * b;
    int dx = b2 * c - b * c2;
    int dy = a * c2 - a2 * c;
    if (d == 0) {
        if (dx == 0 && dy == 0) {
            cout << "Vo so nghiem";
        }
        else {
            cout << "Vo nghiem";
        }
    }
    else {
        cout << "x = " << dx / d << ", y = " << dy / d;
    }

    return 0;
}
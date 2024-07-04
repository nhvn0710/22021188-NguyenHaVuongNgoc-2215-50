#include <iostream>

using namespace std;

int main() {
    int a, b, c;
    cin >> a >> b >> c;
    if (a <= 0 || b <= 0 || c <= 0) cout << "Invalid";
    else {
        if (a + b <= c || a + c <= b || b + c <= a) cout << "Invalid";
        else {
            cout << a + b + c << endl;
            if (a == b && b == c && a == c) cout << "deu";
            else {
                if (a == b || b == c || c == a) cout << "can";
                else {
                    if (a * a + b * b == c * c || a * a + c * c == b * b || b * b + c * c == a * a) {
                        cout << "vuong";
                    }
                    else cout << "thuong";
                }
            }
        }
    }
}
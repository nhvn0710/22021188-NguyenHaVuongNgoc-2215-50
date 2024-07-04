#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

void solveE(double a, double b, double c) {
    double d = b * b - 4 * a * c;
    cout << fixed << setprecision(2);
    if (d > 0) {
        double x1 = (-b - sqrt(d)) / (2 * a);
        double x2 = (-b + sqrt(d)) / (2 * a);
        cout << min(x1, x2) << endl;
        cout << max(x1, x2) << endl;
    }
    else if (d == 0) {
        double x = -b / (2 * a);
        cout << x << endl;
    }
    else {
        double r0 = -b / (2 * a);
        double r1 = sqrt(-d) / (2 * a);
        cout << r0 << " " << -r1 << endl;
        cout << r0 << " " << r1 << endl;
    }
}

int main() {
    double a, b, c;
    cin >> a >> b >> c;
    solveE(a, b, c);

    return 0;
}
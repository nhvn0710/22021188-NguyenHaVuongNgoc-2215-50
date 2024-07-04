#include <iostream>

using namespace std;

void checkMinMax() {
    int n0;
    double tmp;
    cin >> n0 >> tmp;
    double nmin = tmp, nmax = tmp;
    if (n0 > 1) {
        for (int i = 1; i < n0; i++) {
            cin >> tmp;
            if (tmp < nmin) nmin = tmp;
            if (tmp > nmax) nmax = tmp;
        }
    }
    cout << nmax << endl << nmin;
}

int main() {

    checkMinMax();

    return 0;
}
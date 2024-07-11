#include <iomanip>
#include <iostream>

using namespace std;

void writeAverage() {
    int tmp;
    double n = 0;
    for (int i = 0; i < 5; i++)
    {
        cin >> tmp;
        n = n + tmp;
    }
    n = n / 5;
    cout << setprecision(2) << fixed << n;
}

int main() {

    writeAverage();

    return 0;
}
#include <iostream>
#include <string>

using namespace std;

string multiplyBigNumber(const string& a, int b) {
    string outp = "";
    int c = 0;

    for (int i = a.length() - 1; i >= 0; i--) {

        int d = a[i] - '0';
        int t = d * b + c;

        if (t == 0 && outp != "")
        {
            break;
        }
        outp = static_cast<char>((t % 10) + '0') + outp;
        c = t / 10;
    }

    if (c > 0) {
        outp = static_cast<char>(c + '0') + outp;
    }

    return outp;
}

int main() {
    string a;
    int b;

    cin >> a >> b;

    string n = multiplyBigNumber(a, b);
    cout << n << endl;

    return 0;
}
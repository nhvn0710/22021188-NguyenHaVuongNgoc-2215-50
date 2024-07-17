#include <iostream>
#include <string>
#include <vector>

using namespace std;

void makeBinaryNums2(int n, int m, string s, vector<string>& outp) {
    if (n == 0 && m == 0) {
        outp.push_back(s);
        return;
    }
    if (n > 0) { makeBinaryNums2(n - 1, m, s + "0", outp); }
    if (m > 0) { makeBinaryNums2(n, m - 1, s + "1", outp); }
}

void printBinaryNums2()
{
    int n, m;
    cin >> n >> m;
    vector<string> outp;

    makeBinaryNums2(n, m, "", outp);

    for (auto& str : outp) {
        cout << str << endl;
    }
}

int main() {
    printBinaryNums2();
    return 0;
}
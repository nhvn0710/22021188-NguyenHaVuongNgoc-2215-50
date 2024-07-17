#include <iostream>
#include <string>
#include <vector>

using namespace std;

void makeBinaryNums(int n, string s, vector<string>& outp) {
    if (n == 0) {
        outp.push_back(s);
        return;
    }

    makeBinaryNums(n - 1, s + "0", outp);
    makeBinaryNums(n - 1, s + "1", outp);
}

void printBinaryNums()
{
    int n;
    cin >> n;
    vector<string> outp;

    makeBinaryNums(n, "", outp);

    for (auto& str : outp) {
        cout << str << endl;
    }
}

int main() {
    printBinaryNums();
    return 0;
}
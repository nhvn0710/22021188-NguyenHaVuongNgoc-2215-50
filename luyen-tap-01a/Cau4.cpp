#include <iostream>
#include <vector>

using namespace std;

vector<string> ones = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
vector<string> twens = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
vector<string> tenths = { "", "ten", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

void writehundred(int num) {
    if (num > 99) {
        cout << ones[num / 100] << " hundred";
        if (num % 100 != 0) {
            cout << " ";
        }
        num %= 100;
    }

    if (num > 19) {
        cout << tenths[num / 10];
        if (num % 10 != 0) {
            cout << "-" << ones[num % 10];
        }
    }
    else if (num > 0) {
        cout << twens[num];
    }

}

void writenum(int num) {
    if (num == 0) cout << "zero";
    if (num < 0) {
        cout << "negative ";
        num = -num;
    }
    if (num > 999999) {
        writehundred(num / 1000000);
        cout << " million";
        num = num % 1000000;
        if (num > 0) cout << " ";
    }
    if (num > 999) {
        writehundred(num / 1000);
        cout << " thousand";
        num = num % 1000;
        if (num > 0) cout << " ";
    }
    if (num > 0) {
        writehundred(num);
    }

}

int main() {
    int a;
    cin >> a;
    writenum(a);

    return 0;
}
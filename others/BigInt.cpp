#include <cmath>
#include <iostream>
#include <ostream>
using namespace std;

class BigInt
{
    string value;
public:

    BigInt();


    BigInt(const char*);


    BigInt(const BigInt&);


    friend ostream& operator<< (ostream&, const BigInt&);


    friend BigInt operator+(const BigInt&, const BigInt&);
};


ostream& operator<<(ostream& os, const BigInt& num)
{
    os << num.value;
    return os;
}


BigInt::BigInt()
{
    value = "0";
}

BigInt::BigInt(const char* num)
{
    value = num;
}

BigInt::BigInt(const BigInt& num)
{
    value = num.value;
}


BigInt operator+(const BigInt& num, const BigInt& other)
{
    string s1 = num.value;
    string s2 = other.value;
    int n1 = s1.length(), n2 = s2.length();

    if (n1 > n2) {
        s2 = string(n1 - n2, '0') + s2;
    }
    else
    {
        s1 = string(n2 - n1, '0') + s1;
    }

    string s0 = "";
    int p = 0;
    for (int i = n1 - 1; i >= 0; i--) {
        int sum = (s1[i] - '0') + (s2[i] - '0') + p;
        char c = sum % 10 + '0';
        s0 = c + s0;
        p = sum / 10;
    }

    if (p > 0) {
        char c = p + '0';
        s0 = c + s0;
    }
    BigInt outp(s0.c_str());

    return outp;
}

int main() {
    BigInt num("34123");
    cout << num + BigInt("23") + BigInt("3341");
    return 0;
}
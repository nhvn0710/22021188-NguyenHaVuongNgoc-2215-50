#include <iostream>

using namespace std;

int gcd(int a, int b)
{
    if (b == 0)
    {
        return abs(a);
    }
    else
    {
        return gcd(b, a % b);
    }
}

int main() {
    int t, m;
    cin >> t >> m;
    cout << gcd(t, m);
    return 0;
}

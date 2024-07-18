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

int lcm(int a, int b)
{
    return abs(a * b / gcd(a, b));
}

int main() {
    int t, m;
    cin >> t >> m;
    if (t == 0 || m == 0) { cout << 0; }
    else
    {
        cout << lcm(t, m);
    }
    return 0;
}

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

    int d = gcd(t, m);
    t = t / d;
    m = m / d;

    if (m < 0) {
        t = -t;
        m = -m;
    }

    if (m == 1) {
        cout << t;
    }
    else {
        cout << t << "/" << m;
    }

    return 0;
}

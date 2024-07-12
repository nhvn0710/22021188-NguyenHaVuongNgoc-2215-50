#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool checkSystring(string s)
{
    int sl = s.size();
    int i = 0, j = sl - 1;
    while (i < j)
    {
        if (s[i] != s[j]) return false;
        i++;
        j--;
    }
    return true;
}

int main() {
    string s;
    cin >> s;
    if (checkSystring(s))
    {
        cout << "Yes";
    }
    else
    {
        cout << "No";
    }
    return 0;
}
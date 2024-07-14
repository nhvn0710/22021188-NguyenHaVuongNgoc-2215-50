#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool checkSynumber(int i)
{
    string s = "", s0 = to_string(i);
    int sl = s0.size();
    for (int i = 0; i < sl; i++)
    {
        s = s0[i] + s;
    }
    if (s == s0) return true;
    return false;
}

void numofSymnumber(int n)
{
    vector<int> A = {}, B = {}, C = {};
    for (int i = 0; i < n; i++)
    {
        int tmpa, tmpb;
        cin >> tmpa >> tmpb;
        A.push_back(tmpa);
        B.push_back(tmpb);
        C.push_back(0);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = A[i]; j < B[i] + 1; j++)
        {
            if (checkSynumber(j)) C[i]++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        cout << C[i] << endl;
    }
}

int main() {
    int n;
    cin >> n;
    numofSymnumber(n);

    return 0;
}
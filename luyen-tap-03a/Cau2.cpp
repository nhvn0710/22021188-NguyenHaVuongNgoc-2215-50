#include <iostream>
#include <vector>

using namespace std;

bool isNuminArray(int num, vector<int> A)
{
    int left = 0, right = A.size() - 1, mid;
    while (left <= right)
    {
        mid = (right + left) / 2;
        if (num == A[mid]) return true;
        if (num < A[mid])
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return false;
}

void checkNumsinArray()
{
    int n, m;
    cin >> n >> m;
    vector<int> A = {}, B = {};
    for (int i = 0; i < n; i++) {
        int tmp;
        cin >> tmp;
        A.push_back(tmp);
    }
    for (int i = 0; i < m; i++) {
        int tmp;
        cin >> tmp;
        B.push_back(tmp);
    }
    for (int i = 0; i < m; i++) {
        if (isNuminArray(B[i], A)) {
            cout << "YES";
        }
        else
        {
            cout << "NO";
        }
        cout << " ";
    }
}

int main() {
    checkNumsinArray();
    return 0;
}
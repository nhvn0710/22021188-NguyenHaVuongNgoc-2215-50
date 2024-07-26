#include <iostream>
#include <vector>

using namespace std;

bool isConstiSeq(const vector<int>& a, const vector<int>& b) {
    int n = a.size();
    int m = b.size();
    if (n > m) return false;
    for (int i = 0; i <= m - n; i++) {
        bool cdt = true;
        for (int j = 0; j < n; j++) {
            if (a[j] != b[i + j]) {
                cdt = false;
                break;
            }
        }
        if (cdt) return true;
    }
    return false;
}

int main() {
    int n, m;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }

    if (isConstiSeq(a, b)) {
        cout << "YES";
    }
    else {
        cout << "NO";
    }

    return 0;
}
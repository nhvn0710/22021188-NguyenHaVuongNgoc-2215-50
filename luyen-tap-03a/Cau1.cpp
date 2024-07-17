#include <iostream>
#include <vector>

using namespace std;
const int MAXINT = 1000001;

vector<bool> boolPromeArray() {
    vector<bool> isPrime(MAXINT, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i < MAXINT; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j < MAXINT; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return isPrime;
}

void checkPrimeArray()
{
    vector<bool> isPrime = boolPromeArray();
    vector<int> A = {};
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        A.push_back(num);
    }
    for (int i = 0; i < n; i++) {
        if (isPrime[A[i]]) {
            cout << A[i] << " ";
        }
    }
}

int main() {
    checkPrimeArray();
    return 0;
}
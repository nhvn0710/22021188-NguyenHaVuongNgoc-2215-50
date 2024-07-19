//xdd
#include <iostream>
#include <vector>

using namespace std;

void swap0(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void quickSort0(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pindex = (low + high) / 2;
        swap0(arr[pindex], arr[high]);

        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                swap0(arr[i], arr[j]);
            }
        }
        swap0(arr[i + 1], arr[high]);

        pindex = i + 1;

        quickSort0(arr, low, pindex - 1);
        quickSort0(arr, pindex + 1, high);
    }
}

vector<int> sort0(vector<int> arr) {
    if (arr.size() > 1) {
        quickSort0(arr, 0, arr.size() - 1);
    }
    return arr;
}

void printTriplets()
{
    int n;
    cin >> n;
    vector<int> A = {};
    for (int i = 0; i < n; i++) {
        int tmp;
        cin >> tmp;
        A.push_back(tmp);
    }

    if (A.size() < 3) { return; }

    A = sort0(A);
    int tmp1 = A[0] - 1, tmp2 = A[1], tmp3 = A[2];

    for (int i = 0; i < n - 2; i++) {
        int i2 = i + 1;
        int i3 = n - 1;

        while (i2 < i3) {
            int sum = A[i] + A[i2] + A[i3];

            if (sum == 0) {
                if (A[i] != tmp1 || A[i2] != tmp2 || A[i3] != tmp3) {
                    cout << A[i] << " " << A[i2] << " " << A[i3] << endl;
                    tmp1 = A[i], tmp2 = A[i2], tmp3 = A[i3];
                }
                i2++;
                i3--;
            }
            else if (sum < 0) {
                i2++;
            }
            else {
                i3--;
            }

        }
    }
}

int main() {

    printTriplets();
    return 0;
}
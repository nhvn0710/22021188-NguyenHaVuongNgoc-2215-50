#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void minAbDis() {
    int n;
    cin >> n;
    vector<int> sequence(n);
    for (int i = 0; i < n; i++) {
        cin >> sequence[i];
    }
    sort(sequence.begin(), sequence.end());
    int min_distance = abs(sequence[1] - sequence[0]);
    for (int i = 1; i < n; ++i) {
        int distance = abs(sequence[i] - sequence[i - 1]);
        min_distance = min(min_distance, distance);
    }

    cout << min_distance;
}

int main() {
    minAbDis();
    return 0;
}

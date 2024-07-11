#include <iostream>
#include <vector>

using namespace std;

void distinctNumbers() {
    vector<int> numbers;
    int tmp;
    while (true) {
        cin >> tmp;
        numbers.push_back(tmp);
        if (tmp < 0) break;
    }

    if (!numbers.empty()) {
        cout << numbers[0];
        for (size_t i = 1; i < numbers.size(); ++i) {
            if (numbers[i] != numbers[i - 1]) {
                cout << " " << numbers[i];
            }
        }
    }

}

int main() {
    distinctNumbers();
    return 0;
}
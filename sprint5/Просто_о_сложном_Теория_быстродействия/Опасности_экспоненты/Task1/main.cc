#include <cstdint>
#include <iostream>
#include <chrono>

using namespace std;

// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
int64_t T1(int i) {
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }

    return T1(i - 1) + T1(i - 2) + T1(i - 3);
}

int64_t T2(int i) {
    if (i < 3) {
        return i == 2;
    }
    int64_t prev0 = 0, prev1 = 0, prev2 = 1;
    for (int t = 2; t < i; ++t){
        int64_t next = prev0 + prev1 + prev2;
        prev0 = prev1;
        prev1 = prev2;
        prev2 = next;
    }
    return prev2;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }
        {
            auto start = std::chrono::steady_clock::now();
            cout << "Ti1 = "s << T1(i) << endl;
            auto stop = std::chrono::steady_clock::now();
            std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";
        }
        {
            auto start = std::chrono::steady_clock::now();
            cout << "Ti2 = "s << T2(i) << endl;
            auto stop = std::chrono::steady_clock::now();
            std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";
        }
    }
}
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

template <typename InputIt>
void PrintRange(InputIt s, InputIt e) {
    bool first = true;
    while (s != e) {
        if (!first) {
            std::cout << " "s << *s;
        } else {
            std::cout << *s;
            first = false;
        }
        ++s;
    }
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    PrintRange(test.begin(), test.end());
    return 0;
}
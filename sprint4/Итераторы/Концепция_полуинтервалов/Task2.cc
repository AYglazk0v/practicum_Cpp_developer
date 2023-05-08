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
    std::cout << std::endl;
}

template <typename Container, typename T>
void FindAndPrint(const Container& container, const T& value) {
    auto it = std::find(container.begin(), container.end(), value);
    PrintRange(container.begin(), it);
    if (it != container.end()) {
        PrintRange(it, container.end());
    }
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    FindAndPrint(test, 3);
    FindAndPrint(test, 0);
    return 0;
}
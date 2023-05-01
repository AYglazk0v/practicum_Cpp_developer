#include <iostream>
#include <set>
#include <vector>

using namespace std::string_literals;

template <typename T>
void Print(std::ostream& out, const T& containter) {
    for (auto it = containter.begin(), ite = containter.end(); it != ite; ++it) {
        if (it != containter.begin()) {
            out << ", " << *it;
        } else {
            out << *it;
        }
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v){
    Print(out, v);
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s){
    Print(out, s);
    return out;
}

int main() {
    const std::vector<int> ages = {10, 5, 2, 12};
    std::cout << ages << std::endl;

    const std::set<std::string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    std::cout << cats << std::endl;
    return 0;
}

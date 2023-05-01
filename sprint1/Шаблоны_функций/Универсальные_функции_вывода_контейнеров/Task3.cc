#include <iostream>
#include <set>
#include <vector>

using namespace std::string_literals;

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v){
    for (auto it = v.begin(), ite = v.end(); it != ite; ++it) {
        if (it != v.begin()){
            out << ", " << *it;
        } else {
            out << *it;
        }
    }
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& v){
    for (auto it = v.begin(), ite = v.end(); it != ite; ++it) {
        if (it != v.begin()){
            out << ", " << *it;
        } else {
            out << *it;
        }
    }
    return out;
}

int main() {
    const std::vector<int> ages = {10, 5, 2, 12};
    std::cout << ages << std::endl;

    const std::set<std::string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    std::cout << cats << std::endl;
    return 0;
}

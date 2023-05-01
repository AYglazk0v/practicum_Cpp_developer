#include <iostream>
#include <vector>
#include <map>
#include <set>

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

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value>& p){
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::map<Key, Value>& m){
    out << "<";
    Print(out, m);
    out << ">";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v){
    out << "[";
    Print(out, v);
    out << "]";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s){
    out << "{";
    Print(out, s);
    out << "}";
    return out;
}

int main() {
    const std::set<std::string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    std::cout << cats << std::endl;

    const std::vector<int> ages = {10, 5, 2, 12};
    std::cout << ages << std::endl;

    const std::map<std::string, int> cat_ages = {
        {"Мурка"s, 10}, 
        {"Белка"s, 5},
        {"Георгий"s, 2}, 
        {"Рюрик"s, 12}
    };
    std::cout << cat_ages << std::endl;
    return 0;
}
#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v){
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
    return 0;
}

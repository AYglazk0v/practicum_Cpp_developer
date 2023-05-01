#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v){
    for (const auto& curr_elem : v) {
        out << curr_elem << " ";
    }
    return out;
}

int main() {
    const std::vector<int> ages = {10, 5, 2, 12};
    std::cout << ages << std::endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int count;
    std::cin >> count;
    std::vector<std::pair<int, std::string>> humans(count);
    for (int i = 0; i < count; ++i) {
        std::string name;
        int age;
        std::cin >> name >> age;
        humans[i] = {age, name};
    }

    std::sort(humans.begin(), humans.end(), std::greater<std::pair<int, std::string>>());

    for (auto&& curr : humans) {
        std::cout << curr.second << '\n';
    }
}

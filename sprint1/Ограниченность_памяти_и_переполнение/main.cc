#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    const auto min = std::numeric_limits<int64_t>::min();
    const auto max = std::numeric_limits<int64_t>::max();

    std::cout << min << std::endl;
    std::cout << max << std::endl;
    std::cout << min + max << std::endl;
    std::cout << min * 2 << std::endl;
    std::cout << max * 2 << std::endl;
    return 0;
}
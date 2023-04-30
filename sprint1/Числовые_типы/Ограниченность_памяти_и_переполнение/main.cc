#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    {
        const auto min = std::numeric_limits<int64_t>::min();
        const auto max = std::numeric_limits<int64_t>::max();

        std::cout << "min<int64_t> " << min << std::endl;
        std::cout << "max<int64_t> " << max << std::endl;
        std::cout << "min + max<int64_t> "<< min + max << std::endl;
        std::cout << "2 * min<int64_t> "<<min * 2 << std::endl;
        std::cout << "2 * max<int64_t> " << max * 2 << std::endl;
    }
    std::cout << "_____________________________________________\n";
    {
        const auto min = std::numeric_limits<uint64_t>::min();
        const auto max = std::numeric_limits<uint64_t>::max();

        std::cout << "min<uint64_t> " << min << std::endl;
        std::cout << "max<uint64_t> " << max << std::endl;
        std::cout << "min + max<uint64_t> "<< min + max << std::endl;
        std::cout << "2 * min<uint64_t> "<<min * 2 << std::endl;
        std::cout << "2 * max<uint64_t> " << max * 2 << std::endl;
    }
    return 0;
}
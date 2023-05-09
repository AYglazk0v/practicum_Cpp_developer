#include <iostream>
#include <cassert>

int IsPowOfTwo(uint64_t num) {
    if (num == 1) {
        return 1;
    }
    if (num != 0 && num % 2 == 0) {
        return IsPowOfTwo(num / 2);
    } else {
        return 0;
    }
}

void Test() {
    int count_pow_two = 0;
    for (int i = 0; i != 1025; ++i) {
        count_pow_two += IsPowOfTwo(i);
    }
    assert(count_pow_two == 11);
}

int main() {
    Test();

    int result = IsPowOfTwo(1024);
    std::cout << result << std::endl;
    return 0;
}
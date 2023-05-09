#include <iostream>

uint64_t Fibonacci(uint64_t num) {
    if (num <= 1) {
        return num;
    }
    return Fibonacci(num - 1) + Fibonacci(num - 2);
}

int main() {
    std::cout << Fibonacci(6) << std::endl;
    return 0;
}
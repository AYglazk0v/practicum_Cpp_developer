#include <iostream>
#include <string>

using namespace std;

template <typename Function_t>
void RunTestImpl(const Function_t& func, const std::string& function_name) {
    func();
    std::cerr << function_name << " OK"s << std::endl;
}

#define RUN_TEST(func) RunTestImpl((func), #func)

void Test1() {
    std::cout << "RUN TEST1\n"s;
}

int main() {
    RUN_TEST(Test1);
}
#include <iostream>
#include <vector>
#include <algorithm>

void PrintWorryCount(const std::vector<bool>& is_nervous) {
    std::cout << std::count(is_nervous.begin(), is_nervous.end(),true) << std::endl;
}

int main() {
    PrintWorryCount({true, true, false, true});
    return 0;
}

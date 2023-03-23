#include <iostream>
#include <vector>
#include <algorithm>

using namespace std::literals::string_literals;

int CountStartsWithA(const std::vector<std::string>& xs) {
    return std::count_if(xs.begin(), xs.end(), [](const std::string& curr){return (!curr.empty() && (curr[0] == 'a' || curr[0] == 'A'));});
}

int main() {
    std::cout << CountStartsWithA({""s, "And"s, "another"s, "one"s, "gone"s, "another"s, "one"s "bites"s, "the"s, "dust"s});
		return 0;
} 

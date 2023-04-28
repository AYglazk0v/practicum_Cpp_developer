#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

int main() {
    int count_word;
    std::cin >> count_word;
    
    std::vector<std::string> input_str;
    input_str.reserve(count_word);
    for (int i = 0; i != count_word; ++i) {
        std::string input;
        std::cin >> input;
        input_str.emplace_back(std::move(input));
    }

    std::sort(input_str.begin(), input_str.end(), [](
                                const auto& lhs, 
                                const auto& rhs){
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](
                                                        const auto& lc,
                                                         const auto& rc){
            return (std::tolower(lc) < std::tolower(rc));
            });
    });

    std::copy(input_str.begin(), input_str.end(), std::ostream_iterator<std::string>(std::cout, " "));

    return 0;
}
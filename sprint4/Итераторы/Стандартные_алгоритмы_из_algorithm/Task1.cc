#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    return out.str();
}

template <typename It>
std::vector<std::string> GetPermutations(const It& begin,const It& end) {
    std::vector<std::string> ret_val;
    std::sort(begin, end, std::greater<>());
    do {
        ret_val.push_back(PrintRangeToString(begin, end));
    } while (std::prev_permutation(begin, end));
    return ret_val;
}

int main() {
    vector<int> permutation(3);
    iota(permutation.begin(), permutation.end(), 1);

    auto result = GetPermutations(permutation.begin(), permutation.end());
    for (const auto& s : result) {
        cout << s;
    }
    return 0;
}
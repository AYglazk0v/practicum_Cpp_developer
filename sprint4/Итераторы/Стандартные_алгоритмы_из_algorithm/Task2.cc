#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

template <typename InputIt>
void PrintRange(InputIt s, InputIt e) {
    bool first = true;
    while (s != e) {
        if (!first) {
            std::cout << " "s << *s;
        } else {
            std::cout << *s;
            first = false;
        }
        ++s;
    }
    std::cout << std::endl;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    auto dst = std::distance(range_begin, range_end);
    if (dst <= 1) {
        return;
    }
    vector<typename RandomIt::value_type> result(range_begin, range_end);
    auto mid = std::next(result.begin(), dst / 2);
    MergeSort(result.begin(), mid);
    MergeSort(mid, result.end());
    merge(result.begin(), mid, mid, result.end(), range_begin);
}

int main() {
    vector<int> test_vector(10);
    iota(test_vector.begin(), test_vector.end(), 1);
    random_shuffle(test_vector.begin(), test_vector.end());
    PrintRange(test_vector.begin(), test_vector.end());
    MergeSort(test_vector.begin(), test_vector.end());
    PrintRange(test_vector.begin(), test_vector.end());
    return 0;
}
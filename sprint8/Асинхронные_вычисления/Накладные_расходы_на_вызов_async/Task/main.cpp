#include <thread>
#include <random>
#include <future>
#include <vector>
#include <numeric>
#include <iostream>
#include <execution>
#include <algorithm>
#include "log_duration.h"

using namespace std;

int poolThred = [](){
    return std::thread::hardware_concurrency();
}();

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }
    vector elements(range_begin, range_end);
    auto mid = elements.begin() + range_length / 2;

    auto l = [&]{MergeSort(elements.begin(), mid);};
    auto r = [&]{MergeSort(mid, elements.end());};
    
    if (poolThred >= 2) {
        poolThred -= 2;
        auto tmp1 = std::async(l);
        auto tmp2 = std::async(r);
        tmp1.get(), tmp2.get();
        poolThred += 2;
    } else {
        l();
        r();
    }
    merge(std::execution::par, elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    mt19937 generator;

    vector<int> test_vector(100'000);

    iota(test_vector.begin(), test_vector.end(), 1);

    shuffle(test_vector.begin(), test_vector.end(), generator);

    {
        LOG_DURATION("TEST");
        MergeSort(test_vector.begin(), test_vector.end());
    }
    return 0;
}
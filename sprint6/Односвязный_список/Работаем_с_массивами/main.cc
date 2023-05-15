#include <cassert>
#include <vector>
#include <numeric>
#include <iostream>

template <typename T>
void ReverseArray(T* start, size_t size) {
    if (size == 0) {
        return;
    }
    T* end = start + size - 1;
    for (size_t i = 0; i < size / 2; ++i) {
        std::swap(*(start + i), *(end - i));
    }
}

int main() {
    using namespace std;
    
    vector<int> v;
    vector<int> p;
    for (int i = 0; i != 100; ++i) {
        v.reserve(i);
        p = vector<int> {v.rbegin(), v.rend()};
        iota(v.begin(), v.end(), i);
        ReverseArray(v.data(), v.size());
        assert(v == p);
    }
}
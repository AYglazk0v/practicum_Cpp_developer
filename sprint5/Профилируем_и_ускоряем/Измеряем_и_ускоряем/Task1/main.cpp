#include "log_duration.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;

    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

vector<int> ReverseVector2(const vector<int>& source_vector) {
    vector<int> res;

    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator) {
        res.push_back(*iterator);
    }

    return res;
}

vector<int> ReverseVector3(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

vector<int> ReverseVector4(const vector<int>& source_vector) {
    vector<int> res(source_vector.size());
    size_t i = 0;
    for (auto it = source_vector.rbegin(), ite = source_vector.rend(); it != ite; ++it) {
        res[i] = *it;
        ++i;
    }
    return res;
}

template <typename Func>
void LogImpl(const std::string& name, Func func, const std::vector<int>& rand_vec) {
    LOG_DURATION(name);
    std::vector<int> ret = func(rand_vec);
}

void Operate() {
    vector<int> rand_vector;
    int n;

    cin >> n;
    rand_vector.reserve(n);

    for (int i = 0; i < n; ++i) {
        rand_vector.push_back(rand());
    }
    vector<int> rev;
    if (n <= 100'000) {
        LogImpl("Naive"s, ReverseVector, rand_vector);
        LogImpl("Good"s, ReverseVector2, rand_vector);
    } else {
        LogImpl("Good"s, ReverseVector2, rand_vector);
        LogImpl("Best"s, ReverseVector3, rand_vector);
        LogImpl("Your"s, ReverseVector4, rand_vector);

    }
}

int main() {
    Operate();
    return 0;
}
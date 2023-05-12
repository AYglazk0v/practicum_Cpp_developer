#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void FillRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; i += 15) {
        int number = rand();
        
        int count = min(15, n - i);
        
        for (int j = 0; j < count; ++j) {
            v.push_back((number >> j) % 2);
        }
    }
}

void Operate() {
    LOG_DURATION("Total"s);

    vector<int> random_bits;

    static const int N = 1 << 17;

    random_bits.reserve(N);

    {
        LOG_DURATION("Fill random"s);
        FillRandom(random_bits, N);
    }

    vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);

        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
    return 0;
}
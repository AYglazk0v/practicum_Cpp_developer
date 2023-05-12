#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
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

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        v.push_back(rand() % 2);
    }
}

void Operate() {
    LOG_DURATION("Total"s);
    vector<int> random_bits;
    vector<int> reversed_bits;

    static const int N = 1 << 17;

    {
        LOG_DURATION("Append random"s);
        AppendRandom(random_bits, N);
    }
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
}
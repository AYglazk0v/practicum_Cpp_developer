#include "log_duration.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

struct Temp {
    float   accum_temp;
    size_t  count_day;
};

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    if (measures.empty()) {
        return {};
    }
    vector<float> ret(measures.front().size());
    vector<Temp> days(measures.front().size());
    for (size_t i = 0, j; i < measures.size(); ++i) {
        for (j = 0; j < measures.front().size(); ++j) {
            days[j].accum_temp += measures[i][j] > 0 ? measures[i][j] : 0;
            days[j].count_day += measures[i][j] > 0 ? 1 : 0;
        }
    }
    for (size_t i = 0; i != days.size(); ++i) {
        ret[i] = days[i].count_day > 0 ? (days[i].accum_temp / days[i].count_day) : 0;
    }
    return ret;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

void Test() {
    // 4 дня по 3 измерения
    vector<vector<float>> v = {
        {0, -1, -1},
        {1, -2, -2},
        {2, 3, -3},
        {3, 4, -4}
    };

    // среднее для 0-го измерения (1+2+3) / 3 = 2 (не учитывам 0)
    // среднее для 1-го измерения (3+4) / 2 = 3.5 (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    assert(ComputeAvgTemp(v) == vector<float>({2, 3.5f, 0}));
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }
    Test();

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}
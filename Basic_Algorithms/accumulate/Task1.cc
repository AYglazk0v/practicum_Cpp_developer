#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

double Average(const vector<int>& xs) {
    if (xs.empty()) {
        return 0.0;
    }
    return std::accumulate(xs.begin(), xs.end(), 0.0) / (xs.size());
}

int main() {
    cout << Average({1, 2, 2}) << endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    vector<int> cpv1{v1};
    vector<int> cpv2{v2};

    std::sort(cpv1.begin(), cpv1.end());
    std::sort(cpv2.begin(), cpv2.end());

    return cpv1 == cpv2;
}

int main() {
    std::mt19937 g;

    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    std::cout << "Random vectors match? "s << flush;
    std::cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);
    std::cout << "Permuted vectors match? "s << flush;
    std::cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}
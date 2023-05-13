#include <iostream>
#include <string>
#include <numeric>
#include <set>

using namespace std;

inline void increment(std::string& startString_, std::set<char>& alphabet_) {
    for (auto it = startString_.rbegin(), ite = startString_.rend(); it != ite; ++it) {
        auto it_set = alphabet_.lower_bound(*it);
        it_set = std::next(it_set);
        if (it_set == alphabet_.end()) {
            *it = *alphabet_.begin();
        } else {
            *it = *it_set;
        return;
        }
    }
    startString_.insert(startString_.begin(), *alphabet_.begin());
}

template <typename F>
string BruteForce(F check) {
    std::set<char> alphabet;
    for (char i = 'A'; i <= 'Z'; ++i) {
        alphabet.insert(i);
    }
    std::string ret_str {"A"s};
        while (!check(ret_str)) {
            increment(ret_str, alphabet);
        }
    return ret_str;
}

int main() {
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}
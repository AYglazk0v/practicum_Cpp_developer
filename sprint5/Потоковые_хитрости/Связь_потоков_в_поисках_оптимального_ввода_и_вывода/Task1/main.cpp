#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    StreamUntier(std::istream& stream) : istream_(stream) {
        tied_before_ = std::cin.tie(nullptr);
    }
    ~StreamUntier() {
        std::cin.tie(tied_before_);
    }
private:
    istream& istream_;
    ostream* tied_before_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

class MoneyBox {
public:
    explicit MoneyBox(vector<int64_t> nominals)
        : nominals_(move(nominals))
        , counts_(nominals_.size()) {
    }

    const vector<int>& GetCounts() const {
        return counts_;
    }

    void PushCoin(int64_t value) {
        ++counts_[FindIndex_(value)];
    }

    void PrintCoins(ostream& out) const {
        size_t i = 0;
        for (; i != nominals_.size(); ++i) {
            if (counts_[i] > 0) {
                out << nominals_[i] << ": "s << counts_[i] << std::endl;
            }
        }
    }

private:
    int FindIndex_(int nominal) {
        auto it = std::lower_bound(nominals_.begin(), nominals_.end(), nominal);
        // auto it = std::find(nominals_.begin(), nominals_.end(), nominal); Если нет гарантии на порядок
        return std::distance(nominals_.begin(), it);
    }

    const vector<int64_t> nominals_;
    vector<int> counts_;
};

ostream& operator<<(ostream& out, const MoneyBox& cash) {
    cash.PrintCoins(out);
    return out;
}

int main() {
    MoneyBox cash({10, 50, 100, 200, 500, 1000, 2000, 5000});

    int times;
    cout << "Enter number of coins you have:"s << endl;
    cin >> times;

    cout << "Enter all nominals:"s << endl;
    for (int i = 0; i < times; ++i) {
        int64_t value;
        cin >> value;
        cash.PushCoin(value);
    }

    cout << cash << endl;
}
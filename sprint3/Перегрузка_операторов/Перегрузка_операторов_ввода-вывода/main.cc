#include <istream>
#include <numeric>
#include <iostream>
#include <ostream>

using namespace std;

class Rational {
public:
    Rational() = default;

    explicit Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& out, const Rational& rational) {
    out << rational.Numerator() << '/' << rational.Denominator();
    return out;
}

istream& operator>>(istream& in, Rational& rational) {
    int num, denom;
    char delim;
    in >> num >> delim >> denom;
    rational = Rational{num, denom};
    return  in;
}

int main() {
    Rational a;
    std::cin >> a;
    std::cout << std::endl << a;
}

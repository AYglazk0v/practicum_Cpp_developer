#include <iostream>
#include <iterator>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    explicit Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1)
    {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    Rational operator-() {
        return {-numerator_, denominator_};
    }

    Rational operator+() {
        return {numerator_, denominator_};
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

Rational operator+(Rational lhs, Rational rhs) {
    const int numerator = lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator();
    const int denominator = lhs.Denominator() * rhs.Denominator();
    return {numerator, denominator};
}

Rational operator-(Rational lhs, Rational rhs) {
    return {lhs + -rhs};
}

ostream& operator<<(ostream& output, Rational rational) {
    output << rational.Numerator() << '/' << rational.Denominator();
    return output;
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator, denominator;
    char slash;
    input >> numerator >> slash >> denominator;
    rational = Rational{numerator, denominator};
    return input;
}

int main() {
    Rational a;
    Rational b;
    cin >> a >> b;
    std::cout << a + b << " " << a - b << " " << -a << " " << -b << std::endl;
}
#include <iostream>
#include <numeric>
#include <string>

using namespace std;

class Rational {
public:
    Rational() = default;

    explicit Rational(int value)
        : numerator_(value)
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

    Rational& operator+=(Rational rhs) {
        numerator_ = numerator_*rhs.denominator_ + rhs.numerator_ * denominator_;
        denominator_ = denominator_ * rhs.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator-=(Rational rhs) {
        numerator_ = numerator_*rhs.denominator_ - rhs.numerator_ * denominator_;
        denominator_ = denominator_ * rhs.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator*=(Rational rhs) {
        numerator_ = numerator_ * rhs.numerator_;
        denominator_ = denominator_ * rhs.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator/=(Rational rhs) {
        numerator_ = numerator_ * rhs.denominator_;
        denominator_ = denominator_ * rhs.numerator_;
        Normalize();
        return *this;
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

ostream& operator<<(ostream& output, Rational rational) {
    return output << rational.Numerator() << '/' << rational.Denominator();
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator;
    int denominator;
    char slash;
    if ((input >> numerator) && (input >> slash) && (slash == '/') && (input >> denominator)) {
        rational = Rational{numerator, denominator};
    }
    return input;
}

Rational operator+(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

Rational operator-(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

Rational operator+(Rational value) {
    return value;
}

Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}

int main() {
    Rational a(1, 2);
    Rational b(1, 3);
    Rational c = a + b;
    Rational d = a - b;
    // Rational e = a * b;
    // Rational f = a / b;
    
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = a + b = " << c << std::endl;
    std::cout << "d = a - b = " << d << std::endl;
    // std::cout << "e = a * b = " << e << std::endl;
    // std::cout << "f = a / b = " << f << std::endl;
    
    a += b;
    std::cout << "a += b = " << a << std::endl;
    
    a -= b;
    std::cout << "a -= b = " << a << std::endl;
    
    a *= b;
    std::cout << "a *= b = " << a << std::endl;
    
    a /= b;
    std::cout << "a /= b = " << a << std::endl;
    
    return 0;
}
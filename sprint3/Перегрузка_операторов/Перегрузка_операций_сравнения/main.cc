#include <iostream>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int value)
        : numerator_(value)
        , denominator_(1) {
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

    Rational& operator+=(Rational right) {
        numerator_ = numerator_ * right.denominator_ + right.numerator_ * denominator_;
        denominator_ *= right.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator-=(Rational right) {
        numerator_ = numerator_ * right.denominator_ - right.numerator_ * denominator_;
        denominator_ *= right.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator*=(Rational right) {
        numerator_ *= right.numerator_;
        denominator_ *= right.denominator_;
        Normalize();
        return *this;
    }

    Rational& operator/=(Rational right) {
        numerator_ *= right.denominator_;
        denominator_ *= right.numerator_;
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

Rational operator+(Rational value) {
    return value;
}

Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}

Rational operator+(Rational left, Rational right) {
    return left += right;
}

Rational operator-(Rational left, Rational right) {
    return left -= right;
}

Rational operator*(Rational left, Rational right) {
    return left *= right;
}

Rational operator/(Rational left, Rational right) {
    return left /= right;
}

bool operator<(Rational lhs, Rational rhs) {
    return lhs.Numerator()*rhs.Denominator() < rhs.Numerator() * lhs.Denominator();
}

bool operator>(Rational lhs, Rational rhs) {
    return lhs.Numerator()*rhs.Denominator() > rhs.Numerator() * lhs.Denominator();
}

bool operator>=(Rational lhs, Rational rhs) {
    return !(lhs < rhs);
}

bool operator<=(Rational lhs, Rational rhs) {
    return !(lhs > rhs);
}

bool operator==(Rational left, Rational right) {
    return left.Numerator() == right.Numerator() && 
           left.Denominator() == right.Denominator();
}

bool operator!=(Rational left, Rational right) {
    return !(left == right);
}

int main() {
    Rational r1{1, 2};
    Rational r2{2, 3};
    
    std::cout << boolalpha;
    cout << "r1 = " << r1 << ", r2 = " << r2 << endl;
    cout << "r1 < r2: " << (r1 < r2) << endl;
    cout << "r1 <= r2: " << (r1 <= r2) << endl;
    cout << "r1 > r2: " << (r1 > r2) << endl;
    cout << "r1 >= r2: " << (r1 >= r2) << endl;
    cout << "r1 == r2: " << (r1 == r2) << endl;
    cout << "r1 != r2: " << (r1 != r2) << endl;

    std::cout << "_____________________________\n";
    Rational r3{3};
    Rational r4{6, 2};

    cout << "r3 = " << r3 << ", r4 = " << r4 << endl;
    cout << "r3 < r4: " << (r3 < r4) << endl;
    cout << "r3 <= r4: " << (r3 <= r4) << endl;
    cout << "r3 > r4: " << (r3 > r4) << endl;
    cout << "r3 >= r4: " << (r3 >= r4) << endl;
    cout << "r3 == r4: " << (r3 == r4) << endl;
    cout << "r3 != r4: " << (r3 != r4) << endl;

    std::cout << "_____________________________\n";
    Rational r5{1, -2};
    Rational r6{-2, 4};

    cout << "r5 = " << r5 << ", r6 = " << r6 << endl;
    cout << "r5 < r6: " << (r5 < r6) << endl;
    cout << "r5 <= r6: " << (r5 <= r6) << endl;
    cout << "r5 > r6: " << (r5 > r6) << endl;
    cout << "r5 >= r6: " << (r5 >= r6) << endl;
    cout << "r5 == r6: " << (r5 == r6) << endl;
    cout << "r5 != r6: " << (r5 != r6) << endl;
    std::cout << "_____________________________\n";

    return 0;
}
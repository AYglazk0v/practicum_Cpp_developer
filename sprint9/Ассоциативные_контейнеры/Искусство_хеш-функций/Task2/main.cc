#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHash {
    size_t operator()(const Circle& circle) const {
        auto hx = hasher_(circle.x);
        auto hy = hasher_(circle.y);
        auto hr = hasher_(circle.r);
        return hx + hy * 37 + hr * 37 * 37;
    }

private:
    std::hash<double> hasher_;
};

struct DumbbellHash {
    size_t operator()(const Dumbbell& dumbbell) const {
        auto hc1 = circle_hasher_(dumbbell.circle1);
        auto hc2 = circle_hasher_(dumbbell.circle2);
        auto hs = string_hasher_(dumbbell.text);
        return hc1 * (37 * 37 * 37 * 37) + hc2 * (37) + hs;
    }

private:
    CircleHash              circle_hasher_;
    std::hash<std::string>  string_hasher_;
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}
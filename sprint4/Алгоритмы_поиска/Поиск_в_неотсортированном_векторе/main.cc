#include <iostream>

using namespace std;

void PrintSpacesPositions(string& str) {
    for (auto pos = str.find(' '); pos != std::string::npos; pos = str.find(' ', ++pos)) {
        std::cout << pos << std::endl;
    }
}

int main() {
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
    return 0;
}
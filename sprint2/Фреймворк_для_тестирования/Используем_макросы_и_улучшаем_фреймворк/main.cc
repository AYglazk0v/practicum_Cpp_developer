#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

void AssertImpl(bool value, const string& str, const std::string& file, const std::string& func, const unsigned line,  const string& hint) {
    if (!value) {
        std::cerr << file << "("s << line << "): "s << func << ": " << "ASSERT("s << str << ") failed."s;
        if (!hint.empty()) {
            std::cerr << " Hint: "s << hint;
        }
        std::cerr << std::endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl((expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

int main() {
    string hello = "hello"s;
    ASSERT(!hello.empty());
    // ASSERT(hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
}
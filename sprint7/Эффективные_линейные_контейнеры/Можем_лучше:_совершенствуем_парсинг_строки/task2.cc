#include <iostream>
#include <string_view>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        m_forward[std::string{source}] = std::string{target};
        m_backward[std::string{target}] = std::string{source};
    }

    string_view TranslateForward(string_view source) const {
        return m_forward.contains(source.data()) == 0 ? std::string_view() : std::string_view{m_forward.at(source.data())};
    }
    string_view TranslateBackward(string_view target) const {
        return m_backward.contains(target.data()) == 0 ? std::string_view() : std::string_view{m_backward.at(target.data())};
    }

private:
    std::map<std::string, std::string> m_forward;
    std::map<std::string, std::string> m_backward;
};

void TestSimple() {
	Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
}
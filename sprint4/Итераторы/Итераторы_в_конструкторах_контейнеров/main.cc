// разработайте сигнатуру фунции MakeVector по аналогии с фунцией MakeSet из урока
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

template <typename It>
auto MakeVector(It range_begin, It range_end) {
    return std::vector(range_begin, range_end);
}

int main() {
    {
        vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s, "C++"s, "C++"s, "Ruby"s, "Java"s};
        auto unique_langs = MakeSet(langs.begin(), langs.end());
        PrintRange(unique_langs.begin(), unique_langs.end());
    }
    {
        set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
        vector<string> langs = MakeVector(unique_langs.begin(), unique_langs.end());
        PrintRange(langs.begin(), langs.end());
    }
    return 0;
}
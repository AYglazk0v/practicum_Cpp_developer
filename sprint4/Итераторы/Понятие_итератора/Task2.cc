#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    auto it = std::find_if(langs.begin(), langs.end(), [](const Lang& l){return l.name[0] == 'J';});
    std::cout << it->name << ", "s << it->age << '\n';
    auto it_range = std::ranges::find_if(langs, [](const Lang& l){return l.name[0] == 'J';});
    std::cout << it_range->name << ", "s << it_range->age << '\n';
    return 0;
}
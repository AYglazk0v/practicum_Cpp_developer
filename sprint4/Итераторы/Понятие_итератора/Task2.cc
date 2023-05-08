#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    auto it = std::find_if(langs.begin(), langs.end(), [](const Lang& l){return l.name[0] == 'J';});
    std::cout << it->name << ", "s << it->age;
    return 0;
}
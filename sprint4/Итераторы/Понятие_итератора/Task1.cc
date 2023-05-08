#include <algorithm>
#include <ranges>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    // Выведите первый язык, начинающийся на C, используя алгоритм find_if
    std::cout << *std::find_if(langs.begin(), langs.end(), [](const std::string& s){return s[0] == 'C';}) << '\n';
    std::cout << *std::ranges::find_if(langs, [](const std::string& s){return s[0] == 'C';});
    return 0;
}
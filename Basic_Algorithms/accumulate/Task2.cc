#include <iostream>
#include <set>
#include <numeric>

using namespace std;
string AddStopWords(const string& query, const set<string>& stop_words) {
    return std::accumulate(stop_words.begin(), stop_words.end(), query, [](std::string acc, std::string add){return acc += " -"s + add;});
}

int main() {
    std::set<std::string> stop_words{"of", "in"};
    std::string query = "some tasty oranges";

    std::cout << AddStopWords(query, stop_words) << std::endl;
}

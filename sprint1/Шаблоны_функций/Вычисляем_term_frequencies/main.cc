#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

map<string, int> ComputeTermFreqs(const vector<string>& terms) {
    std::map<std::string, int> return_map;
    for (auto& curr_word : terms) {
        ++return_map[curr_word];
    }
    return return_map;
    // реализуйте функцию
}

int main() {
    const vector<string> terms = {"first"s, "time"s, "first"s, "class"s};
    for (const auto& [term, freq] : ComputeTermFreqs(terms)) {
        cout << term << " x "s << freq << endl;
    }
		// вывод:
		// class x 1
		// first x 2
		// time x 1
}
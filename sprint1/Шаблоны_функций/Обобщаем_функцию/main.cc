#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

template <typename Term>
pair<Term, int> FindMaxFreqAnimal(const vector<pair<Term, int>>& animals) {
    auto ret_m = ComputeTermFreqs(animals);

    std::pair<Term,int> ret_pair;
    int max = 0;
    for (auto& current_elem : ret_m) {
        if (current_elem.second > max) {
            ret_pair = current_elem.first;
            max = current_elem.second;
        }
    }
    return ret_pair;
}

int main() {
    const vector<pair<string, int>> animals = {
        {"Murka"s, 5},
        {"Belka"s, 6},
        {"Murka"s, 7},
        {"Murka"s, 5}, 
        {"Murka"s, 5}, 
        {"Belka"s, 6},
    };
    const pair<string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    cout << max_freq_animal.first << " "s
         << max_freq_animal.second << endl;
}
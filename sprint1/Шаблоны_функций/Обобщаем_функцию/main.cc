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
    std::pair<Term,int> ret_pair;
    int max_freq = 0;
    for (const auto& [animal, freq] : ComputeTermFreqs(animals)) {
        if (freq > max_freq) {
            ret_pair = animal;
            max_freq = freq;
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
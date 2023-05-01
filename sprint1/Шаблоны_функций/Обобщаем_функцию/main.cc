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
    return *std::max_element(animals.begin(), animals.end(), [](const std::pair<Term, int>& lhs, const std::pair<Term, int>& rhs) {return lhs.second > rhs.second;});
}

int main() {
    const vector<pair<string, int>> animals = {
        {"Murka"s, 5},
        {"Belka"s, 6},
        {"Murka"s, 7},
        {"Murka"s, 5}, 
        {"Belka"s, 6},
    };
    const pair<string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    cout << max_freq_animal.first << " "s
         << max_freq_animal.second << endl;
    
    const vector<pair<int, int>> leg_count = {
        {4, 5},
        {4, 6},
        {2, 7},
        {4, 5}, 
        {1, 6},
    };
    const pair<int, int> max_legs = FindMaxFreqAnimal(leg_count);
    cout << max_legs.first << " "s
         << max_legs.second << endl;
}
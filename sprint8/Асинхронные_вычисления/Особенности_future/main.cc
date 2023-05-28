#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats& other) {
        for (auto& [word, freq] : other.word_frequences) {
            word_frequences[word] += freq;
        }
    }
};

using KeyWords = set<string, less<>>;

Stats ExploreKeyWords(const KeyWords& key_words, istream& input) {
    if (!input.good()) {
        return {};
    }

    std::string str;
    std::getline(input, str);
    auto worker = std::async([&key_words, &input]{return ExploreKeyWords(key_words, input);});

    Stats stats{};
    auto pos_alpha = str.find_first_not_of(' ');
    auto pos_delim = str.find_first_of(' ', pos_alpha);
    while (pos_alpha != str.npos || pos_delim != str.npos) {
        std::string tmp = str.substr(pos_alpha, pos_delim - pos_alpha);
        if (key_words.count(tmp)) {
            ++stats.word_frequences[tmp];
        }
        pos_alpha = str.find_first_not_of(' ', pos_delim);
        pos_delim = str.find_first_of(' ', pos_alpha);
    }
    stats += worker.get();
    return stats;
}

int main() {
    const KeyWords key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto& [word, frequency] : ExploreKeyWords(key_words, ss).word_frequences) {
        cout << word << " " << frequency << endl;
    }

    return 0;
}
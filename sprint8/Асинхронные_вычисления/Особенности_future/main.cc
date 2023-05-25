#include <functional>
#include <future>
#include <iostream>
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
    input >> str;
    auto worker = std::async([&key_words, &input]{return ExploreKeyWords(key_words, input);});

    Stats stats{};
    for (auto it = str.begin(), it0 = str.begin(), ite = str.end(); it != ite;) {
        for (; it0 != ite && isprint(*it0); ++it0);
        auto it_found_key = key_words.find({it, it0});
        if (it_found_key != key_words.end()) {
            ++stats.word_frequences[*it_found_key];
        }
        it = it0;
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
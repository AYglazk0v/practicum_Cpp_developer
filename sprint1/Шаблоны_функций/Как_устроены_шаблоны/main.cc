#include <iostream>
#include <cmath>
#include <vector>

using namespace std::string_literals;

template <typename Documents,typename Term>
std::vector<double> ComputeTfIdfs (const Documents& docs, const Term& term) {
    std::vector<double> ret;
    ret.reserve(docs.size());

    int freq = 0;
    for (const auto& doc : docs) {
        if (std::find(doc.begin(), doc.end(), term) != doc.end()) {
            ++freq;
        }
    }

    double idf = std::log(docs.size()/static_cast<double>(freq));
    
    for (const auto& doc : docs) {
        ret.push_back(idf * (std::count(doc.begin(), doc.end(), term)/static_cast<double>(doc.size())));
    }

    return ret;
}

int main() {
    const std::vector<std::vector<std::string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        std::cout << tf_idf << " "s;
    }
    std::cout << std::endl;
}
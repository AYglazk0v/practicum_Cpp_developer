#include "remove_duplicates.h"

#include <set>
#include <map>
#include <iostream>

void RemoveDuplicates(SearchServer& search_server) {
    using namespace std::string_literals;

    std::set<int> duplicates;
    std::set<std::set<std::string_view>> docs;
    for (const auto doc_id : search_server) {
        const auto& doc = search_server.GetWordFrequencies(doc_id);
        std::set<std::string_view> prob_doc;
        for (const auto& [word, freq] : doc) {
            prob_doc.insert(word);
        }
        if (docs.count(prob_doc) == 1) {
            duplicates.insert(doc_id);
        } else {
            docs.insert(prob_doc);
        }
    }

    for (auto id : duplicates) {
        std::cout << "Found duplicate document id "s << id << std::endl;
        search_server.RemoveDocument(id);
    }
}
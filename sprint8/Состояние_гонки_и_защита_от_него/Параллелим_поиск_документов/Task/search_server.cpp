#include "search_server.h"

#include <cmath>
#include <numeric>
#include <functional>

SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text)) {
}

SearchServer::SearchServer(std::string_view stop_words_text)
    : SearchServer(SplitIntoWordsView(stop_words_text)) {
}

void SearchServer::AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings) {
    using namespace std::string_literals;
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id"s);
    }
    
    auto words = SplitIntoWordsNoStop(document);
    
    const std::string document_string{document};
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status, document_string});
    words = SplitIntoWordsNoStop(documents_.at(document_id).text);

    const double inv_word_count = 1.0 / words.size();
    for (auto word : words) {
        word_to_document_freqs_[word][document_id] += inv_word_count;
        id_to_word_freqs_[document_id][word] += inv_word_count;
    }
    document_ids_.insert(document_id);
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentStatus status) const {
    return FindTopDocuments(raw_query,
                            [&status](int document_id, DocumentStatus document_status, int rating) {
                                return document_status == status;
                            });
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const{
    static const std::map<std::string_view, double> map;
    const auto result = id_to_word_freqs_.find(document_id);
    return result == id_to_word_freqs_.end() ? map : result->second;
}

void SearchServer::RemoveDocument(int document_id) {
    const auto id_to_word_freqs = id_to_word_freqs_.find(document_id);
    if (id_to_word_freqs != id_to_word_freqs_.end())
    {
        for (const auto& [word, freqs] : id_to_word_freqs->second) {
            word_to_document_freqs_.erase(word);
        }
        documents_.erase(document_id);
        document_ids_.erase(document_id);
        id_to_word_freqs_.erase(id_to_word_freqs);
    }
}

bool SearchServer::IsStopWord(std::string_view word) const {
    return stop_words_.count(word) > 0;
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(std::string_view raw_query, int document_id) const {
    const auto query = ParseQuery(raw_query);
    std::vector<std::string_view> matched_words;
    for (auto word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    for (auto word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.clear();
            break;
        }
    }
    return {matched_words, documents_.at(document_id).status};
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(std::string_view text) const {
    using namespace std::string_literals;
    std::vector<std::string_view> words;
    for (auto word : SplitIntoWordsView(text)) {
        if (!IsValidWord(word)) {
            throw std::invalid_argument("Word "s + std::string{word} + " is invalid"s);
        }
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

double SearchServer::ComputeWordInverseDocumentFreq(std::string_view word) const {
    return std::log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view text) const {
    using namespace std::string_literals;
    if (text.empty()) {
        throw std::invalid_argument("Query word is empty"s);
    }
    bool is_minus = false;
    if (text[0] == '-') {
        is_minus = true;
        text = text.substr(1);
    }
    if (text.empty() || text[0] == '-' || !IsValidWord(text)) {
        throw std::invalid_argument("Query word "s + std::string{text} + " is invalid");
    }

    return {text, is_minus, IsStopWord(text)};
}

SearchServer::Query<std::set<std::string_view, std::less<>>> SearchServer::ParseQuery(std::string_view text) const {
    Query result;
    for (auto word : SplitIntoWordsView(text)) {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.insert(query_word.data);
            } else {
                result.plus_words.insert(query_word.data);
            }
        }
    }
    return result;
}

SearchServer::Query<std::vector<std::string_view>> SearchServer::ParseQueryParallel(std::string_view text) const {
    Query<std::vector<std::string_view>> result;
    for (auto word : SplitIntoWordsView(text)) {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.push_back(query_word.data);
            } else {
                result.plus_words.push_back(query_word.data);
            }
        }
    }
    return result;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) const {
    if (ratings.empty()) {
        return 0;
    }
    return std::accumulate(ratings.begin(), ratings.end(),
         0, std::plus<int>()) / static_cast<int>(ratings.size());
}
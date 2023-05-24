#pragma once

#include <execution>
#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>

#include "document.h"
#include "string_processing.h"
#include "log_duration.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
    {
        using namespace std::string_literals;
        if (!all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
            throw std::invalid_argument("Some of stop words are invalid"s);
        }
    }

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {
        const auto query = ParseQuery(raw_query);

        auto matched_documents = FindAllDocuments(query, document_predicate);
    
    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            const double EPSILON = 1e-6;
            if (std::abs(lhs.relevance - rhs.relevance) < EPSILON) {
                return lhs.rating > rhs.rating;
            }
            else {
                return lhs.relevance > rhs.relevance;
            }
        });
    
        std::sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        return matched_documents;
    }

public:
    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

public:

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        std::map<int, double> document_to_relevance;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        for (const std::string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
        std::vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }   

private:
    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };
    QueryWord ParseQueryWord(const std::string& text) const;

public:
    explicit SearchServer(const std::string& stop_words_text);

public:
    void AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings);
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const;
    std::vector<Document> FindTopDocuments(const std::string& raw_query) const;
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const;

    Query ParseQuery(const std::string& text) const;
    bool IsStopWord(const std::string& word) const;
    int GetDocumentCount() const;

    void RemoveDocument(int document_id);
    const std::map<std::string, double>& GetWordFrequencies(int document_id) const;

    auto begin() const {
        return document_ids_.begin();
    }

    auto end() const {
        return document_ids_.end();
    }

    template <typename ExecutionPolicy>
    void RemoveDocument(ExecutionPolicy execut, int document_id) {
        const auto id_to_word_freqs = id_to_word_freqs_.find(document_id);
        if (id_to_word_freqs != id_to_word_freqs_.end())
        {
            std::for_each(execut, id_to_word_freqs->second.begin(), id_to_word_freqs->second.end(),
                [this](auto& w_f){
                    word_to_document_freqs_.erase(w_f.first);
            });
            documents_.erase(document_id);
            document_ids_.erase(document_id);
            id_to_word_freqs_.erase(id_to_word_freqs);
        }
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

private:
    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;
    double ComputeWordInverseDocumentFreq(const std::string& word) const;

private:
    static int ComputeAverageRating(const std::vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

private:
    const std::set<std::string>                     stop_words_;
    std::map<std::string, std::map<int, double>>    word_to_document_freqs_;
    std::map<int, DocumentData>                     documents_;
    std::set<int>                                   document_ids_;
    std::map<int, std::map<std::string, double>>    id_to_word_freqs_;
};
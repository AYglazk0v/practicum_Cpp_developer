#pragma once

#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <iostream>
#include <execution>
#include <functional>
#include <algorithm>

#include "document.h"
#include "string_processing.h"
#include "log_duration.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer {
    public:
        explicit SearchServer(const std::string& stop_words_text);
        explicit SearchServer(std::string_view stop_words_text);

        template <typename StringContainer>
        explicit SearchServer(const StringContainer& stop_words)
            : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
            using namespace std::string_literals;
            if (!all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
                throw std::invalid_argument("Some of stop words are invalid"s);
            }
        }

    public:
        template <typename Container = std::set<std::string_view, std::less<>>>
        struct Query {
            Container plus_words;
            Container minus_words;
        };

    private:
        struct QueryWord {
            std::string_view data;
            bool is_minus;
            bool is_stop;
        };

        struct DocumentData {
            int rating;
            DocumentStatus status;
        };

    public:
        void AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings);
    
        template <typename DocumentPredicate>
        std::vector<Document> FindAllDocuments(const Query<>& query, DocumentPredicate document_predicate) const;

        template <typename DocumentPredicate>
        std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentPredicate document_predicate) const;
        std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentStatus status) const;
        std::vector<Document> FindTopDocuments(std::string_view raw_query) const;


        template <typename ExecutionPolicy>
        std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(ExecutionPolicy&& execut, std::string_view raw_query, int document_id) const;
        std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::string_view raw_query, int document_id) const;

        template <typename ExecutionPolicy>
        void RemoveDocument(ExecutionPolicy&& execut, int document_id);
        void RemoveDocument(int document_id);

        QueryWord ParseQueryWord(std::string_view text) const;
        Query<std::set<std::string_view, std::less<>>> ParseQuery(std::string_view text) const;
        Query<std::vector<std::string_view>> ParseQueryParallel(std::string_view text) const;

        int GetDocumentCount() const;
        bool IsStopWord(std::string_view word) const;
        const std::map<std::string_view, double>& GetWordFrequencies(int document_id) const;

        auto begin() const      { return document_ids_.begin(); }
        auto end() const        { return document_ids_.end(); }

    private:
        std::vector<std::string_view> SplitIntoWordsNoStop(std::string_view text) const;
        double ComputeWordInverseDocumentFreq(std::string_view word) const;
        int ComputeAverageRating(const std::vector<int>& ratings) const;

    private:
        const std::set<std::string, std::less<>>                stop_words_;
        std::map<std::string_view, std::map<int, double>>       word_to_document_freqs_;
        std::map<int, std::map<std::string_view, double>>       id_to_word_freqs_;
        std::map<int, DocumentData>                             documents_;
        std::set<int>                                           document_ids_;
};
    
    
template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentPredicate document_predicate) const {
    const auto query = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(query, document_predicate);
    sort(matched_documents.begin(), matched_documents.end(),
    [](const Document& lhs, const Document& rhs) {
        if (std::abs(lhs.relevance - rhs.relevance) < 1e-6) {
            return lhs.rating > rhs.rating;
        }
        else {
            return lhs.relevance > rhs.relevance;
        }
    });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query<>& query, DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;
    for (auto word : query.plus_words) {
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
    for (auto word : query.minus_words) {
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

template <typename ExecutionPolicy>
std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(ExecutionPolicy&& execut, std::string_view raw_query, int document_id) const {
    auto query = ParseQueryParallel(raw_query);
    std::vector<std::string_view> matched_words;
    if (any_of(execut, query.minus_words.begin(), query.minus_words.end(),
    [&](const auto& word) {
        return (word_to_document_freqs_.count(word)
            && word_to_document_freqs_.at(word).count(document_id));
    })) {
        return {matched_words, documents_.at(document_id).status};
    }
    matched_words.resize(query.plus_words.size());
    const auto& it = std::copy_if(execut,
        query.plus_words.begin(), query.plus_words.end(),
        matched_words.begin(),
        [&](const auto& word) {
            return (word_to_document_freqs_.count(word)
                && word_to_document_freqs_.at(word).count(document_id));
    });
    matched_words.erase(it, matched_words.end());
    std::sort(execut, matched_words.begin(), matched_words.end());
    const auto& it_un = std::unique(execut, matched_words.begin(), matched_words.end());
    matched_words.erase(it_un, matched_words.end());
    
    return {matched_words, documents_.at(document_id).status};
}

template <typename ExecutionPolicy>
void SearchServer::RemoveDocument(ExecutionPolicy&& execut, int document_id) {
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

#pragma once
#include "search_server.h"
#include "document.h"

#include <vector>
#include <deque>

class RequestQueue {
public:
    RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto query = search_server_.ParseQuery(raw_query);
        auto matched_documents = search_server_.FindAllDocuments(query, document_predicate);
        AddRequest(matched_documents.size());
        return matched_documents;
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;

private:
    void AddRequest(int results_num);

private:
    struct QueryResult {
        explicit QueryResult(const int result) : results_(result) {
        }        
        int results_;
    };

    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    SearchServer search_server_;
    int current_time_ = 0;
    int request_older_ = 1;
};
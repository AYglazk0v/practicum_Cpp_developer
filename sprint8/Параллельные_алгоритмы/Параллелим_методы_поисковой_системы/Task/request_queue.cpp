#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server) {
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddFindRequest(raw_query, [status](int document_id, DocumentStatus document_status, int rating){
        return document_status == status;
    });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}

int RequestQueue::GetNoResultRequests() const {
    return requests_.size() - request_older_;
}

void RequestQueue::AddRequest(int results_num) {
    ++current_time_;
    if (current_time_ > min_in_day_) {
        requests_.pop_front();
        --current_time_;
        ++request_older_;
    }
    requests_.emplace_back(results_num);
}
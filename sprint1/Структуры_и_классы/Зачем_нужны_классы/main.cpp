#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class SearchServer {
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
public:
    SearchServer() = default;
    explicit SearchServer(std::map<std::string, std::set<int>>& w, std::set<string>& s) : word_to_documents_(w), stop_words_(s) {};
    int GetStopWordsSize() const {
        return stop_words_.size();
    }
	
};

int GetStopWordsSize(const SearchServer& server) {
    return server.GetStopWordsSize();
}

int main() {
    std::map<std::string, std::set<int>> w{{"a",{1}},{"b",{2}},{"c",{3}}};
    std::set<std::string> s {"a","b","c"};
    SearchServer server;
    std::cout << "method=" << server.GetStopWordsSize() << " func=" << GetStopWordsSize(server)  << std::endl;
    server = SearchServer{w, s};
    std::cout << "method=" << server.GetStopWordsSize() << " func=" << GetStopWordsSize(server)  << std::endl;
    return 0;
}
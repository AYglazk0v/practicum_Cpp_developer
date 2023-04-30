#include <iostream>

using namespace std;


class SearchServer {
public:
    enum class DocumentStatus {
        ACTUAL,
        IRRELEVANT,
        BANNED,
        REMOVED,
    };
};

int main() {
    std::cout << static_cast<int>(SearchServer::DocumentStatus::BANNED);
    return 0;
}
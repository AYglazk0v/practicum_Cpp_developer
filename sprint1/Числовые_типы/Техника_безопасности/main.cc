#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
}; 

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
        if (documents.size() < skip_start || documents.size() < skip_finish)
            return;
        std::sort(documents.begin(), documents.end(), [](const Document& lhs, const Document& rhs){
            return lhs.rating > rhs.rating;
        });
        
        for (size_t i = skip_start; i < skip_finish; ++i) {
            std::cout << "{ id = " << documents[i].id << ", rating = " << documents[i].rating << " }\n";
        }
}

int main() {
    PrintDocuments(
        {
            {100, 5},
            {101, 7},
            {102, -4},
            {103, 9},
            {104, 1}
        },
        1,
        2
    );
}  

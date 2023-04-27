#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Document {
    int id;
    int relevance;
};


vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            words.push_back(word);
            word = ""s;
        } else {
            word += c;
        }
    }
    words.push_back(word);

    return words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

// For each document returns its relevance and id
vector<Document> FindAllDocuments(
				const map<string, set<int>>& word_to_documents,
				const set<string>& stop_words,
				const string& query) {
	
		const vector<string> query_words = SplitIntoWordsNoStop(query, stop_words);
		map<int, int> document_to_relevance;
		for (const string& word : query_words) {
				if (word_to_documents.count(word) == 0) {
						continue;
				}
				for (const int document_id : word_to_documents.at(word)) {
						++document_to_relevance[document_id];
				}
		}
	
		vector<Document> matched_documents;
		for (auto [document_id, relevance] : document_to_relevance) {
				matched_documents.push_back({relevance, document_id});
		}
		
		return matched_documents;
}

int main() {
    return 0;
}
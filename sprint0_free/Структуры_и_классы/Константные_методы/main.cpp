#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <execution>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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
			word = "";
		} else {
			word += c;
		}
	}
	words.push_back(word);
	return words;
}

class SearchServer {
	public:
		void AddDocument(int document_id, const std::string& document) {
			for (auto& word : SplitIntoWordsNoStop(document)) {
				word_to_documents_[word].insert(document_id);
			}
		}

		vector<Document> FindTopDocuments (
				const string& query) const {
			
			auto matched_documents = FindAllDocuments(query);
	
			sort(execution::par, matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs){
				if (rhs.relevance != lhs.relevance) {
					return lhs.relevance > rhs.relevance;
				}
				return lhs.id > rhs.id;
			});

			if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
				matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
			}
			return matched_documents;
		}

		void SetStopWords(const string& text) {
			for (const string& word : SplitIntoWords(text)) {
				stop_words_.insert(word);
			}
		}

	private:
		vector<string> SplitIntoWordsNoStop(const string& text) const {
			vector<string> words;
			for (const string& word : SplitIntoWords(text)) {
				if (stop_words_.count(word) == 0) {
					words.push_back(word);
				}
			}
			return words;
		}

		vector<Document> FindAllDocuments(const string& query) const {
			const vector<string> query_words = SplitIntoWordsNoStop(query);
			map<int, int> document_to_relevance;
			for (const string& word : query_words) {
				if (word_to_documents_.count(word) == 0) {
					continue;
				}
				for (const int document_id : word_to_documents_.at(word)) {
					++document_to_relevance[document_id];
				}
			}
			vector<Document> matched_documents;
			for (auto [document_id, relevance] : document_to_relevance) {
				matched_documents.emplace_back(document_id, relevance);
			}
			return matched_documents;
		}

	private:
		std::map<std::string, std::set<int>> word_to_documents_;
		std::set<std::string> stop_words_;
};

string ReadLine() {
	string s;
	getline(cin, s);
	return s;
}

int ReadLineWithNumber() {
	int result;
	cin >> result;
	ReadLine();
	return result;
}

SearchServer CreateSearchServer() {
	SearchServer ret;
	const string stop_words_joined = ReadLine();
	ret.SetStopWords(stop_words_joined);

	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; ++document_id) {
		ret.AddDocument(document_id, ReadLine());
	}
	return ret;
}

int main() {
	const SearchServer search_server = CreateSearchServer();
	
	const string query = ReadLine();

	for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
		cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
	}
}
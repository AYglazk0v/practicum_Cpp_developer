#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <cmath>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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

struct Document {
		int id;
		double relevance;
};

struct Query{
		std::set<std::string> word_plus_;
		std::set<std::string> word_minus_;
};

class SearchServer final {
public:
		void SetStopWords(const string& text) {
				for (const string& word : SplitIntoWords(text)) {
						stop_words_.insert(word);
				}
		}

		void AddDocument(int document_id, const string& document) {
				auto tmp_vec = SplitIntoWordsNoStop(document);
				for (const string& word : tmp_vec) {
						word_to_documents_freq_[word][document_id] += 1.0/tmp_vec.size();
				}
				++count_docs_;
		}

		vector<Document> FindTopDocuments(const string& query) const {
				auto matched_documents = FindAllDocuments(query);
				
				sort(
						matched_documents.begin(),   
						matched_documents.end(), 
						[](const Document& lhs, const Document& rhs) {
								return lhs.relevance > rhs.relevance;
						}
				);
				if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
					matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
				}
				matched_documents.erase(std::remove_if(matched_documents.begin(), matched_documents.end(),[](Document& val){return val.relevance <= 0.0;}), matched_documents.end());
				return matched_documents;
		}

private:
		map<string, map<int, double>>	word_to_documents_freq_;
    	set<string>						stop_words_;
		int								count_docs_;

private:
		Query ParseQuery(std::vector<std::string> query) const {
			Query ret_query;
			for (auto& curr_w : query) {
				if (curr_w[0] == '-') {
					ret_query.word_minus_.insert(curr_w.substr(1));
				} else {
					ret_query.word_plus_.insert(curr_w);
				}
			}
			return ret_query;	
		}

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
				Query query_words = ParseQuery(SplitIntoWordsNoStop(query));
				map<int, double> document_to_relevance;
				for (const string& word : query_words.word_plus_) {
						if (word_to_documents_freq_.count(word) == 0) {
								continue;
						}
						for (auto [id, freq]: word_to_documents_freq_.at(word)) {
							document_to_relevance[id] += freq * std::log((count_docs_ + 0.0)/ word_to_documents_freq_.at(word).size());
						}
				}

				for (const string& word : query_words.word_minus_) {
						if (word_to_documents_freq_.count(word) == 0) {
							continue;
						}
						for (auto [id, freq]: word_to_documents_freq_.at(word)) {
							document_to_relevance[id] -= freq * std::log((count_docs_ + 0.0) / word_to_documents_freq_.at(word).size());
						}
				}
				
				vector<Document> matched_documents;
				for (auto [document_id, relevance] : document_to_relevance) {
						matched_documents.push_back({document_id, relevance});
				}
				return matched_documents;
		}
};

SearchServer CreateSearchServer() {
		SearchServer search_server;
		search_server.SetStopWords(ReadLine());
	
		const int document_count = ReadLineWithNumber();
		for (int document_id = 0; document_id < document_count; ++document_id) {
				search_server.AddDocument(document_id, ReadLine());
		}
		
		return search_server;
}


int main() {
		const SearchServer search_server = CreateSearchServer();
	
		const string query = ReadLine();
		for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
				cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
		}
}

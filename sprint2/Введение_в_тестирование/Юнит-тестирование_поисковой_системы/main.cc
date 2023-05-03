#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <cassert>
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
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
    
struct Document {
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }    
    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, 
            DocumentData{
                ComputeAverageRating(ratings), 
                status
            });
    }
    
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status = DocumentStatus::ACTUAL) const {
        const auto cmp = [&status](const int id, const DocumentStatus& d_status, const int raitind){ return status == d_status;};
        return FindTopDocuments(raw_query, cmp);
    }

    template<typename CmpPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, const CmpPredicate& cmp) const {            
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, cmp);
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
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

    int GetDocumentCount() const {
        return documents_.size();
    }
    
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
    }
    
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }
    
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template<typename Cmp>
    vector<Document> FindAllDocuments(const Query& query, const Cmp& cmp) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                if (cmp(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)){
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({
                document_id,
                relevance,
                documents_.at(document_id).rating
            });
        }
        return matched_documents;
    }
};


void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating
         << " }"s << endl;
}

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

//Проверка фукнции GetDocumentCount
void TestCountDocs() {
    //Убеждаемся, что изначально количество документов равно нулю
    {
        SearchServer server;
        assert(server.GetDocumentCount() == 0);
    }

    //Добавляем документ и проверяем, что количество изменилось
    {
        const int id {42};
        const std::string content {"London is capital of the Great Britan"s};
        const vector<int> ratings {1, 2, 3};

        SearchServer server;
        server.AddDocument(id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.GetDocumentCount() == 1);
    }

    //Добавляем документы с одним ID и проверяем их количество
    {
        const int id {42};
        const std::string content{"London is capital of the Great Britan"s};
        const std::string content1{"London is capital of the Russia "s};
        const vector<int> ratings = {1, 2, 3};
        const vector<int> ratings1 = {1, 2};

        SearchServer server;
        server.SetStopWords("007"s);
        server.AddDocument(id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(id, content1, DocumentStatus::ACTUAL, ratings1);
        assert(server.GetDocumentCount() == 1);
    }
}

//Проверка на минус слова
void TestMinusWord() {
    const int id {42};
    const std::string content = "Уважаемые товарищи потомки! Роясь в сегодняшнем окаменевшем говне наших дней изучая потемки, вы, возможно, спросите и обо мне."s;
    const vector<int> rating {7,7,7};
    
    //Пустая структура, если минус-слово отбрасывает единственный подходящий документ
    {
        SearchServer server;
        server.AddDocument(id, content, DocumentStatus::ACTUAL, rating);
        assert(server.FindTopDocuments("в -говне").empty() == true);
    }

    //Отбрасывается только документ содержащий минус-слово
    {
        const int second_id {21};
        const std::string second_content = "Я, ассенизатор и водовоз, революцией мобилизованный и призванный, ушел на фронт из барских садоводств поэзии — бабы капризной."s;

        SearchServer server;
        server.AddDocument(id, content, DocumentStatus::ACTUAL, rating);
        server.AddDocument(second_id, second_content, DocumentStatus::ACTUAL, rating);
        assert(server.FindTopDocuments("и -говне").size() == 1u);
    }

    //Проверка, что без минус-слов запрос отрабатывает корректно
    {
        const int second_id {21};
        const std::string second_content = "Я, ассенизатор и водовоз, революцией мобилизованный и призванный, ушел на фронт из барских садоводств поэзии — бабы капризной."s;

        SearchServer server;
        server.AddDocument(id, content, DocumentStatus::ACTUAL, rating);
        server.AddDocument(second_id, second_content, DocumentStatus::ACTUAL, rating);
        assert(server.FindTopDocuments("и").size() == 2u);
    }
}

//Проверка функции Matched
void TestMatchedDocumentContent() {
    {
        SearchServer server;
        server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
        server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});

        std::tuple tuple_with_word_and_status = server.MatchDocument("пушистый ухоженный белый кот"s, 0);
        assert(std::get<0>(tuple_with_word_and_status).size() == 2u);

        vector<string> expected_response = {"белый", "кот"};
        assert(std::get<0>(tuple_with_word_and_status) == expected_response);
    }

    {
        SearchServer server;
        server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
        server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});

        std::tuple tuple_with_word_and_status = server.MatchDocument("пушистый ухоженный -кот"s, 1);
        assert(std::get<0>(tuple_with_word_and_status).empty() == true);
    }
}

//Проверка сортировки по релевантности
void TestSortRelevance() {
    SearchServer server;

    server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});
    server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});

    double last_relevance = 0.0;
    for (const Document& document : server.FindTopDocuments("пушистый ухоженный кот"s)) {
        assert(document.relevance > last_relevance);
    }
}

void TestCountReturnTopDocument(){
    SearchServer server;

    server.AddDocument(0, "a b s"s, DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(1, "a b s"s, DocumentStatus::ACTUAL, {7,7,7,});
    server.AddDocument(2, "a b s"s, DocumentStatus::ACTUAL, {9,9,9});
    server.AddDocument(3, "a b s"s, DocumentStatus::ACTUAL, {-1,-1,-1});
    server.AddDocument(4, "a b s"s, DocumentStatus::ACTUAL, {0});
    server.AddDocument(5, "a b s"s, DocumentStatus::ACTUAL, {5,5,5});

    assert(MAX_RESULT_DOCUMENT_COUNT == 5);
    assert(server.FindTopDocuments("a"s, DocumentStatus::ACTUAL).size() == 5);
}

//Проверка, что выводится средний рейтинг
void TestAverageRating() {
    SearchServer server;
    
    server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -3});
    auto doc = server.FindTopDocuments("кот"s);
    assert(doc[0].rating == (8 - 3)/2);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestCountDocs();
    TestMinusWord();
    TestSortRelevance();
    TestAverageRating();
    TestMatchedDocumentContent();
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}
<details>  
<summary>Теория:</summary>

# Обработка ошибок в поисковой системе

Исключения позволяют отделить код, выполняющий полезную работу, от кода обработки ошибок. Этим они делают его чище. Более того, в С++ бывают ситуации, когда без исключений не обойтись:

-   Сообщение вызывающему коду об ошибке, которая обнаружена в конструкторе класса. Задача конструктора — обеспечить нужные инварианты для создаваемого объекта класса. Исключения позволяют сообщить, что создать объект с валидным состоянием невозможно. Так пользователям не придётся взаимодействовать с непригодным объектом.
-   Сообщение об ошибках во время работы операций над пользовательскими типами.

Теперь вы знаете достаточно, чтобы усовершенствовать обработку ошибок в поисковой системе.

</details>  

<details>  
<summary>Задание:</summary>

## Задание

Это задание — итоговый проект третьего спринта. Вы будете сдавать его на проверку через репозиторий на GitHub.

Доработайте класс  `SearchServer`. Примените механизм исключений и реализуйте в  `SearchServer`  обработку проблем.

-   Конструкторы класса  `SearchServer`  должны выбрасывать исключение  `invalid_argument`, если любое из переданных стоп-слов содержит недопустимые символы, то есть символы с кодами от 0 до 31. Такого требования не было в предыдущих заданиях, так как известные вам на тот момент способы обработки ошибок не позволяли эффективно решить эту задачу в конструкторе.
-   Метод  `AddDocument`  больше не должен использовать возврат значения типа  `bool`  для сообщения об успехе или ошибке. Вместо этого он должен выбрасывать исключение  `invalid_argument`  в следующих ситуациях:
    -   Попытка добавить документ с отрицательным  `id`;
    -   Попытка добавить документ c  `id`  ранее добавленного документа;
    -   Наличие недопустимых символов (с кодами от 0 до 31) в тексте добавляемого документа.
-   Методы  `FindDocument`  вместо возврата  `optional<vector<Document>>`  должны возвращать  `vector<Document>`  и выбрасывать исключение  `invalid_argument`  в следующих ситуациях:
    -   В словах поискового запроса есть недопустимые символы с кодами от 0 до 31;
    -   Наличие более чем одного минуса перед словами, которых не должно быть в искомых документах, например,  `пушистый --кот`. В середине слов минусы разрешаются, например:  `иван-чай`.
    -   Отсутствие текста после символа «минус»: в поисковом запросе:  `"пушистый -"`.
-   Метод  `MatchDocument`  должен возвращать  `tuple<vector<string>, DocumentStatus>`, выбрасывая исключение  `invalid_argument`  в тех же ситуациях, что и метод  `FindDocument`.
-   Метод  `GetDocumentId`  должен выбрасывать исключение  `out_of_range`, если индекс переданного документа выходит за пределы допустимого диапазона (0; количество документов).

```cpp
class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words) {
        ...
    }

    explicit SearchServer(const string& stop_words_text) {
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        ...
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        ...
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        ...
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        ...
    }

    int GetDocumentCount() const {
        ...
    }

    int GetDocumentId(int index) const {
        ...
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        ...
    }
    ...
};

// ------------ Пример использования ----------------

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) {
    cout << "{ "s
         << "document_id = "s << document_id << ", "s
         << "status = "s << static_cast<int>(status) << ", "s
         << "words ="s;
    for (const string& word : words) {
        cout << ' ' << word;
    }
    cout << "}"s << endl;
}

void AddDocument(SearchServer& search_server, int document_id, const string& document, DocumentStatus status,
                 const vector<int>& ratings) {
    try {
        search_server.AddDocument(document_id, document, status, ratings);
    } catch (const exception& e) {
        cout << "Ошибка добавления документа "s << document_id << ": "s << e.what() << endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const string& raw_query) {
    cout << "Результаты поиска по запросу: "s << raw_query << endl;
    try {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) {
            PrintDocument(document);
        }
    } catch (const exception& e) {
        cout << "Ошибка поиска: "s << e.what() << endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const string& query) {
    try {
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch (const exception& e) {
        cout << "Ошибка матчинга документов на запрос "s << query << ": "s << e.what() << endl;
    }
}

int main() {
    SearchServer search_server("и в на"s);

    AddDocument(search_server, 1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    AddDocument(search_server, 1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    AddDocument(search_server, -1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    AddDocument(search_server, 3, "большой пёс скво\x12рец евгений"s, DocumentStatus::ACTUAL, {1, 3, 2});
    AddDocument(search_server, 4, "большой пёс скворец евгений"s, DocumentStatus::ACTUAL, {1, 1, 1});

    FindTopDocuments(search_server, "пушистый -пёс"s);
    FindTopDocuments(search_server, "пушистый --кот"s);
    FindTopDocuments(search_server, "пушистый -"s);

    MatchDocuments(search_server, "пушистый пёс"s);
    MatchDocuments(search_server, "модный -кот"s);
    MatchDocuments(search_server, "модный --пёс"s);
    MatchDocuments(search_server, "пушистый - хвост"s);
}

```

### Подсказка

Выбор стратегии обработки ошибок влияет на архитектуру класса в целом. Поэтому выбирайте стратегию на этапе проектирования класса. Обработка ошибок с использованием кодов ошибок или  `optional`  пронизывает все методы класса — от публичных до приватных.

При переходе на механизм обработки исключений внесите изменения в сигнатуру методов, которые ранее возвращали код ошибки. Чтобы упростить себе задачу, сначала переведите на механизм исключений публичные методы, потом приватные методы, от которых зависят только что переведённые публичные. Когда внесёте изменения в очередной метод, проверьте, что программа компилируется и корректно работает.

При реализации обработки ошибок в методе  `GetDocumentId`  вам может пригодиться то, что метод  `at`  в классе  `vector`  выбрасывает исключение  `out_of_range`, если передать в качестве индекса недопустимое значение.

</details>  

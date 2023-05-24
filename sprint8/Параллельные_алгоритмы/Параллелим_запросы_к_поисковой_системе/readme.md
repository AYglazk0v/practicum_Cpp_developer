<details>
<summary>Теория:</summary>

# Параллелим запросы к поисковой системе

При написании параллельных программ нужно осторожно относиться к использованию общей памяти. В последующих темах вы увидите, как попытка изменить один и тот же объект из разных потоков может приводить к ошибкам или даже к аварийному завершению программы.

Однако нет ничего опасного в том, чтобы читать одни и те же данные из разных потоков, не меняя их. Это вам сейчас и предстоит — вы будете одновременно искать несколько запросов в поисковой системе. Модификатор  `const`  поможет понять, что вы на верном пути: работая из разных потоков только с константными методами объекта, можете быть спокойны. Бывают редкие случаи, когда такие методы всё же меняют внутреннее состояние объекта — но это уже забота автора класса.

</details>

<details>
<summary>Задание 1:</summary>

## Задание 1

Это задание — часть итогового проекта восьмого спринта. Оно продолжает проект по разработке поисковой системы из пятого спринта: возьмите решение оттуда в качестве основы. Задание вы будете сдавать на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Напишите функцию  `ProcessQueries`, распараллеливающую обработку нескольких запросов к поисковой системе. Объявление функции:

```cpp
std::vector<std::vector<Document>> ProcessQueries(
    const SearchServer& search_server,
    const std::vector<std::string>& queries);

```

Она принимает N запросов и возвращает вектор длины N, i-й элемент которого — результат вызова  `FindTopDocuments`  для i-го запроса.

### Пример

```cpp
#include "process_queries.h"
#include "search_server.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    SearchServer search_server("and with"s);

    int id = 0;
    for (
        const string& text : {
            "funny pet and nasty rat"s,
            "funny pet with curly hair"s,
            "funny pet and not very nasty rat"s,
            "pet with rat and rat and rat"s,
            "nasty rat with curly hair"s,
        }
    ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, {1, 2});
    }

    const vector<string> queries = {
        "nasty rat -not"s,
        "not very funny nasty pet"s,
        "curly hair"s
    };
    id = 0;
    for (
        const auto& documents : ProcessQueries(search_server, queries)
    ) {
        cout << documents.size() << " documents for query ["s << queries[id++] << "]"s << endl;
    }

    return 0;
}

```

Вывод:

```
3 documents for query [nasty rat -not]
5 documents for query [not very funny nasty pet]
2 documents for query [curly hair]

```

### Ограничения

Поисковые запросы корректны и не порождают исключений.

### Что отправлять на проверку

Заголовочные файлы и файлы с реализацией, которые содержат класс  `SearchServer`, написанные ранее вспомогательные функции и новую функцию  `ProcessQueries`. Функция  `main`  будет проигнорирована.

### Как будет тестироваться ваш код

Правильность работы функции будет проверена юнит-тестами.

Время работы вашей функции должно быть по крайней мере вдвое меньше, чем у тривиального решения:

```cpp
    for (const std::string& query : queries) {
        documents_lists.push_back(search_server.FindTopDocuments(query));
    }

```

Мы проверим это на таких входных данных:

-   поисковая система из 20 000 документов, не более 10 слов в каждом;
-   2 000 поисковых запросов, не более 7 слов в каждом;
-   все слова — из словаря, состоящего из 2 000 слов длиной не более 25 букв.

Вы можете ориентироваться на следующий бенчмарк:

```cpp
#include "search_server.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "log_duration.h"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

vector<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, max_length));
    }
    sort(words.begin(), words.end());
    words.erase(unique(words.begin(), words.end()), words.end());
    return words;
}

string GenerateQuery(mt19937& generator, const vector<string>& dictionary, int max_word_count) {
    const int word_count = uniform_int_distribution(1, max_word_count)(generator);
    string query;
    for (int i = 0; i < word_count; ++i) {
        if (!query.empty()) {
            query.push_back(' ');
        }
        query += dictionary[uniform_int_distribution<int>(0, dictionary.size() - 1)(generator)];
    }
    return query;
}

vector<string> GenerateQueries(mt19937& generator, const vector<string>& dictionary, int query_count, int max_word_count) {
    vector<string> queries;
    queries.reserve(query_count);
    for (int i = 0; i < query_count; ++i) {
        queries.push_back(GenerateQuery(generator, dictionary, max_word_count));
    }
    return queries;
}

template <typename QueriesProcessor>
void Test(string_view mark, QueriesProcessor processor, const SearchServer& search_server, const vector<string>& queries) {
    LOG_DURATION(mark);
    const auto documents_lists = processor(search_server, queries);
}

#define TEST(processor) Test(#processor, processor, search_server, queries)

int main() {
    mt19937 generator;
    const auto dictionary = GenerateDictionary(generator, 2'000, 25);
    const auto documents = GenerateQueries(generator, dictionary, 20'000, 10);

    SearchServer search_server(dictionary[0]);
    for (size_t i = 0; i < documents.size(); ++i) {
        search_server.AddDocument(i, documents[i], DocumentStatus::ACTUAL, {1, 2, 3});
    }

    const auto queries = GenerateQueries(generator, dictionary, 2'000, 7);
    TEST(ProcessQueries);
}

```

### Подсказка

Достаточно применить функцию  `transform`, которая запишет результат в вектор, заранее созданный с нужной длиной.

</details>

<details>
<summary>Задание 2:</summary>

## Задание 2

Это задание — часть итогового проекта восьмого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Напишите функцию  `ProcessQueriesJoined`. Она должна, подобно функции  `ProcessQueries`, распараллеливать обработку нескольких запросов к поисковой системе, но возвращать набор документов в плоском виде. Объявление функции:

```cpp
/* набор объектов Document */ ProcessQueriesJoined(
    const SearchServer& search_server,
    const std::vector<std::string>& queries);

```

Функция должна вернуть объект  `documents`. Для него можно написать  `for (const Document& document : documents)`  и получить сначала все документы из результата вызова  `FindTopDocuments`  для первого запроса, затем для второго и так далее. Количество итераций такого цикла должно быть равно суммарному размеру внутренних векторов, возвращаемых функцией  `ProcessQueries`.

Конкретный тип возвращаемого значения остаётся на ваше усмотрение: это может быть как  `vector<Document>`  или  `list<Document>`, так и ваш собственный тип данных с определёнными методами  `begin`  и  `end`. Главное — чтобы работал  `for`, и скорость была приемлемой.

### Пример

```cpp
#include "process_queries.h"
#include "search_server.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    SearchServer search_server("and with"s);

    int id = 0;
    for (
        const string& text : {
            "funny pet and nasty rat"s,
            "funny pet with curly hair"s,
            "funny pet and not very nasty rat"s,
            "pet with rat and rat and rat"s,
            "nasty rat with curly hair"s,
        }
    ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, {1, 2});
    }

    const vector<string> queries = {
        "nasty rat -not"s,
        "not very funny nasty pet"s,
        "curly hair"s
    };
    for (const Document& document : ProcessQueriesJoined(search_server, queries)) {
        cout << "Document "s << document.id << " matched with relevance "s << document.relevance << endl;
    }

    return 0;
}

```

Вывод:

```
Document 1 matched with relevance 0.183492
Document 5 matched with relevance 0.183492
Document 4 matched with relevance 0.167358
Document 3 matched with relevance 0.743945
Document 1 matched with relevance 0.311199
Document 2 matched with relevance 0.183492
Document 5 matched with relevance 0.127706
Document 4 matched with relevance 0.0557859
Document 2 matched with relevance 0.458145
Document 5 matched with relevance 0.458145

```

### Ограничения

Поисковые запросы корректны и не порождают исключений.

### Что отправлять на проверку

Заголовочные файлы и файлы с реализацией, содержащие класс  `SearchServer`, написанные ранее вспомогательные функции и новую функцию  `ProcessQueriesJoined`. Функция  `main`  будет проигнорирована.

### Как будет тестироваться ваш код

Правильность работы функции будет проверена юнит-тестами.

Как и в предыдущей задаче, время работы вашей функции должно быть по крайней мере вдвое меньше, чем у тривиального решения с последовательными вызовами  `FindTopDocuments`.

Мы проверим это на таких входных данных:

-   поисковая система из 20 000 документов, не более 10 слов в каждом;
-   2 000 поисковых запросов, не более 7 слов в каждом;
-   все слова — из словаря, состоящего из 2 000 слов длиной не более 25 букв.

### Подсказка

Попробуйте разные варианты:

-   как можно проще переиспользовать предыдущее решение;
-   применить вместо вектора легко объединяемый контейнер;
-   и что угодно ещё, что подскажет вам фантазия.

</details>

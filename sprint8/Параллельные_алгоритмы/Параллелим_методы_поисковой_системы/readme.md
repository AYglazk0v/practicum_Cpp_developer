<details>
<summary>Теория:</summary>

# Параллелим методы поисковой системы

Часто действительно достаточно распараллелить обработку нескольких запросов — и дело в шляпе. Но бывает и другая ситуация: один запрос обрабатывается слишком долго, и распараллелить нужно его реализацию.

Было бы неправильно, если бы по умолчанию все методы работали параллельно. Ведь в таком случае функция  `ProcessQueries`  будет порождать потоки на двух уровнях: при обработке нескольких запросов и для каждого конкретного запроса. В следующей теме вы увидите, что это неэффективно.

Поэтому при распараллеливании конкретных методов класса  `SearchServer`:

-   сохраните прежнее поведение, когда метод вызывается по-старому или с  `std::execution::seq`;
-   используйте многопоточную реализацию, если первым аргументом передан  `std::execution::par`  — как это сделано для стандартных алгоритмов.

Тип этих специальных объектов вы можете подсмотреть в документации. Использовать сами объекты в функции не нужно.

</details>

<details>
<summary>Задание 1:</summary>

## Задание 1

Это задание — часть итогового проекта восьмого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Реализуйте многопоточную версию метода  `RemoveDocument`  в дополнение к однопоточной.

### Пример

```cpp
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

    const string query = "curly and funny"s;

    auto report = [&search_server, &query] {
        cout << search_server.GetDocumentCount() << " documents total, "s
            << search_server.FindTopDocuments(query).size() << " documents for query ["s << query << "]"s << endl;
    };

    report();
    // однопоточная версия
    search_server.RemoveDocument(5);
    report();
    // однопоточная версия
    search_server.RemoveDocument(execution::seq, 1);
    report();
    // многопоточная версия
    search_server.RemoveDocument(execution::par, 2);
    report();

    return 0;
}

```

Вывод:

```
5 documents total, 4 documents for query [curly and funny]
4 documents total, 3 documents for query [curly and funny]
3 documents total, 2 documents for query [curly and funny]
2 documents total, 1 documents for query [curly and funny]

```

### Ограничения

Как и прежде, в метод  `RemoveDocument`  может быть передан любой  `document_id`. Если такой документ отсутствует, метод должен немедленно завершиться.

### Что отправлять на проверку

Заголовочные файлы и файлы с реализацией, содержащие класс  `SearchServer`  и написанные ранее вспомогательные функции. Функция  `main`  будет проигнорирована.

### Как будет тестироваться ваш код

Правильность работы метода будет проверена юнит-тестами.

Производительность всех версий метода будет проверена в таком тестовом сценарии. Дана поисковая система с характеристиками:

-   10 000 документов, не более 100 слов в каждом;
-   все слова — из словаря, состоящего из 10 000 слов длиной не более 25 букв.

Из этой системы последовательно удаляются все документы. Измеряется общее время работы цикла вызовов  `RemoveDocument`.

Для многопоточной версии вашего метода это время должно быть по крайней мере на 30% меньше, чем у авторской однопоточной версии.

Также это время должно быть по крайней мере на 30% меньше, чем у вашей однопоточной версии в обоих вариантах вызова.

Вы можете ориентироваться на следующий бенчмарк:

```cpp
#include "search_server.h"

#include <execution>
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

template <typename ExecutionPolicy>
void Test(string_view mark, SearchServer search_server, ExecutionPolicy&& policy) {
    LOG_DURATION(mark);
    const int document_count = search_server.GetDocumentCount();
    for (int id = 0; id < document_count; ++id) {
        search_server.RemoveDocument(policy, id);
    }
    cout << search_server.GetDocumentCount() << endl;
}

#define TEST(mode) Test(#mode, search_server, execution::mode)

int main() {
    mt19937 generator;

    const auto dictionary = GenerateDictionary(generator, 10'000, 25);
    const auto documents = GenerateQueries(generator, dictionary, 10'000, 100);

    {
        SearchServer search_server(dictionary[0]);
        for (size_t i = 0; i < documents.size(); ++i) {
            search_server.AddDocument(i, documents[i], DocumentStatus::ACTUAL, {1, 2, 3});
        }

        TEST(seq);
    }
    {
        SearchServer search_server(dictionary[0]);
        for (size_t i = 0; i < documents.size(); ++i) {
            search_server.AddDocument(i, documents[i], DocumentStatus::ACTUAL, {1, 2, 3});
        }

        TEST(par);
    }
}

```

### Подсказка

Вы можете столкнуться с тем, что нужный алгоритм не параллелится, когда передаёте в него итераторы не произвольного доступа. Попробуйте переложить нужные элементы в вектор и запустить алгоритм для него.

</details>

<details>
<summary>Задание 2:</summary>

## Задание 2

Это задание — часть итогового проекта восьмого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Реализуйте многопоточную версию метода  `MatchDocument`  в дополнение к однопоточной.

### Пример

```cpp
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

    const string query = "curly and funny -not"s;

    {
        const auto [words, status] = search_server.MatchDocument(query, 1);
        cout << words.size() << " words for document 1"s << endl;
        // 1 words for document 1
    }

    {
        const auto [words, status] = search_server.MatchDocument(execution::seq, query, 2);
        cout << words.size() << " words for document 2"s << endl;
        // 2 words for document 2
    }

    {
        const auto [words, status] = search_server.MatchDocument(execution::par, query, 3);
        cout << words.size() << " words for document 3"s << endl;
        // 0 words for document 3
    }

    return 0;
}

```

Вывод:

```
1 words for document 1
2 words for document 2
0 words for document 3

```

### Ограничения

Как и прежде, в метод  `MatchDocument`  может быть передан некорректный поисковый запрос — в этом случае должно быть выброшено исключение  `std::invalid_argument`.

Если передан несуществующий  `document_id`, ожидается исключение  `std::out_of_range`.

### Что отправлять на проверку

Заголовочные файлы и файлы с реализацией, содержащие класс  `SearchServer`  и написанные ранее вспомогательные функции. Функция  `main`  будет проигнорирована.

### Как будет тестироваться ваш код

Правильность работы метода будет проверена юнит-тестами.

Производительность всех версий метода будет проверена в таком тестовом сценарии. Дана поисковая система с характеристиками:

-   10 000 документов, не более 70 слов в каждом;
-   не более 500 слов в поисковом запросе, включая минус-слова;
-   все слова — из словаря, состоящего из 1 000 слов длиной не более 10 букв.

Дан поисковый запрос, состоящий из не более 500 слов, включая минус-слова.

Последовательно для всех документов вызывается  `MatchDocument`  с этим запросом. Измеряется общее время работы цикла вызовов.

Для многопоточной версии вашего метода это время должно быть по крайней мере на 30% меньше, чем у авторской однопоточной версии.

Также это время должно быть по крайней мере на 30% меньше, чем у вашей однопоточной версии в обоих вариантах вызова.

Вы можете ориентироваться на следующий бенчмарк:

```cpp
#include "search_server.h"

#include <execution>
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

string GenerateQuery(mt19937& generator, const vector<string>& dictionary, int word_count, double minus_prob = 0) {
    string query;
    for (int i = 0; i < word_count; ++i) {
        if (!query.empty()) {
            query.push_back(' ');
        }
        if (uniform_real_distribution<>(0, 1)(generator) < minus_prob) {
            query.push_back('-');
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

template <typename ExecutionPolicy>
void Test(string_view mark, SearchServer search_server, const string& query, ExecutionPolicy&& policy) {
    LOG_DURATION(mark);
    const int document_count = search_server.GetDocumentCount();
    int word_count = 0;
    for (int id = 0; id < document_count; ++id) {
        const auto [words, status] = search_server.MatchDocument(policy, query, id);
        word_count += words.size();
    }
    cout << word_count << endl;
}

#define TEST(policy) Test(#policy, search_server, query, execution::policy)

int main() {
    mt19937 generator;

    const auto dictionary = GenerateDictionary(generator, 1000, 10);
    const auto documents = GenerateQueries(generator, dictionary, 10'000, 70);

    const string query = GenerateQuery(generator, dictionary, 500, 0.1);

    SearchServer search_server(dictionary[0]);
    for (size_t i = 0; i < documents.size(); ++i) {
        search_server.AddDocument(i, documents[i], DocumentStatus::ACTUAL, {1, 2, 3});
    }

    TEST(seq);
    TEST(par);
}

```

### Подсказка

Иногда для получения эффективного параллельного кода недостаточно поменять код самого метода. Приходится вносить изменения в другие методы, от которых зависит оптимизируемый. Скорее всего, здесь как раз тот случай.

Не любое ускорение означает именно распараллеливание. Вероятно, вам поможет взглянуть на задачу чуть шире — как на приведение в соответствие бенчмарку.

</details>

<details>
<summary>Задание 3:</summary>

## Задание 3

Это задание — часть итогового проекта восьмого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Иногда ускорение требует существенной переработки кода программы. Сейчас — тот самый случай. Перейдите с использования класса  `string`  на  `string_view`  там, где это возможно, и тем самым ускорьте программу.

Следующие методы теперь должны позволять принять  `string_view`  вместо строки:

-   конструктор;
-   AddDocument;
-   FindTopDocuments;
-   MatchDocument.

Эти методы должны возвращать  `string_view`  вместо строк:

-   MatchDocument;
-   GetWordFrequencies.

### Ограничения

В метод  `MatchDocument`  может быть передан некорректный поисковый запрос — в этом случае должно быть выброшено исключение  `std::invalid_argument`.

Если передан несуществующий  `document_id`, ожидается исключение  `std::out_of_range`.

### Что отправлять на проверку

Заголовочные файлы и файлы с реализацией, содержащие класс  `SearchServer`  и написанные ранее вспомогательные функции. Функция  `main`  будет проигнорирована.

### Как будет тестироваться ваш код

Правильность работы метода будет проверена юнит-тестами.

Производительность всех версий метода будет проверена в таком тестовом сценарии. Дана поисковая система с характеристиками:

-   10 000 документов, не более 70 слов в каждом;
-   не более 500 слов в поисковом запросе, включая минус-слова;
-   все слова — из словаря, состоящего из 1 000 слов длиной не более 10 букв.

Дан поисковый запрос , состоящий из не более 500 слов, включая минус-слова.

Последовательно для всех документов вызывается  `MatchDocument`  с этим запросом. Измеряется общее время работы цикла вызовов.

Время работы вашей многопоточной версии должно быть по крайней мере вдвое меньше, чем время работы вашей однопоточной версии в обоих вариантах вызова.

Время работы вашей однопоточной версии не должно превышать время работы однопоточной версии авторского решения больше, чем в полтора раза.

Время работы вашей многопоточной версии не должно превышать время работы многопоточной версии авторского решения больше, чем в полтора раза.

### Подсказка

Помните:  `string_view`  не владеет строкой. Поэтому там, где нужно, используйте  `string`  или ссылайтесь на копию переданных в метод данных.

Вы можете искать  `string_view`  во множестве строк  `set<string>`  без создания временной строки, но для этого вам нужно явно в самом типе множества указать специальный компаратор:  `set<string, less<>>`.

</details>

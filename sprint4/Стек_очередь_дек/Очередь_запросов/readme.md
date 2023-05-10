<details>
<summary>Теория:</summary>

# Очередь запросов

В прошлом уроке вы узнали, что для упорядочивания часто поступающих запросов их организуют в очередь. Это полезно в нескольких случаях.

-   Когда запросов приходит слишком много, их обработка занимает время. Запросы, ожидающие обработки, могут просто «‎посидеть»‎ в очереди и подождать, пока сервис-обработчик доберётся до них.
-   Для хранения только нужных запросов. Например, вы хотите знать, какие запросы пользователи отправляли на поисковый сервер. Но важны только актуальные запросы за последние сутки. То есть вам интересно время отправки. Хранить запросы старше суток не требуется.

Каждую минуту приходит один запрос от пользователя. То есть максимальное количество запросов, которые надо хранить, — это количество минут в сутках (1440). Появление 1441 запроса будет означать, что сутки прошли, первый запрос прошлых суток нам больше не интересен и может быть удалён. Для реализации такого механизма удобно использовать  `deque`. Новый запрос легко вставится в конец, а устаревший запрос удалится из начала.

</details>

<details>
<summary>Задание:</summary>

## Задание

Сохраните решение этой задачи: локально или в  `git`. Оно вам ещё понадобится.

Напишите класс  `RequestQueue`, принимающий запросы на поиск, —  `AddFindRequest`. Класс  `RequestQueue`  должен уметь ответить на вопрос, сколько за последние сутки было запросов, на которые ничего не нашлось. Заготовка класса будет выглядеть так:

```cpp
class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server) {
        // напишите реализацию
    }
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        // напишите реализацию
    }

    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status) {
        // напишите реализацию
    }

    vector<Document> AddFindRequest(const string& raw_query) {
        // напишите реализацию
    }

    int GetNoResultRequests() const {
        // напишите реализацию
    }
private:
    struct QueryResult {
        // определите, что должно быть в структуре
    };
    deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    // возможно, здесь вам понадобится что-то ещё
};

```

### Пример использования

```cpp
int main() {
    SearchServer search_server("and in at"s);
    RequestQueue request_queue(search_server);

    search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, {1, 1, 1});

    // 1439 запросов с нулевым результатом
    for (int i = 0; i < 1439; ++i) {
        request_queue.AddFindRequest("empty request"s);
    }
    // все еще 1439 запросов с нулевым результатом
    request_queue.AddFindRequest("curly dog"s);
    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    request_queue.AddFindRequest("big collar"s);
    // первый запрос удален, 1437 запросов с нулевым результатом
    request_queue.AddFindRequest("sparrow"s);
    cout << "Total empty requests: "s << request_queue.GetNoResultRequests() << endl;
    return 0;
}

```

### Пример вывода

Запросов, по которым ничего не нашлось 1437.

### Подсказка

В момент появления нового запроса:

-   Увеличьте время на одну секунду (запросы приходят раз в секунду);
-   Удалите из дека все запросы, которые успели устареть;
-   Не забудьте во время удаления уменьшить количество запросов с пустым вектором ответов, если нужно;
-   Добавьте новый запрос в дек и обновите количество запросов без результатов поиска.

</details>

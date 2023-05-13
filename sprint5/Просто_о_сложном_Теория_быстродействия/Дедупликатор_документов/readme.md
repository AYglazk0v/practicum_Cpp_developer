<details>
<summary>Теория:</summary>

# Дедупликатор документов

Поисковые системы сталкиваются с проблемой — зеркалами. Это копии сайта. Их количество в Интернете может достигать десятков или сотен. Чтобы первые страницы поисковой выдачи не состояли из копий одного и того же сайта, нужно разработать дедупликатор. Он удаляет копии из поискового сервера. Но поскольку функция удаления пока не предусмотрена, вам предстоит расширить поисковый сервер.

</details>

<details>
<summary>Задание:</summary>

## Задание

Это задание — итоговый проект пятого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. Не забудьте сохранить верное решение.

Расширьте поисковый сервер, добавив в него дополнительные методы.

1.  Откажитесь от метода  `GetDocumentId(int index)`  и вместо него определите методы  `begin`  и  `end`. Они вернут итераторы. Итератор даст доступ к id всех документов, хранящихся в поисковом сервере. Вы можете не разрабатывать собственный итератор, а применить готовый константный итератор удобного контейнера.
    
    Если  `begin`  и  `end`  определены корректно, появится возможность использовать упрощённую форму  `for`  с поисковым сервером:
    
    ```cpp
    SearchServer search_server;
    
    
    for (const int document_id : search_server) {
        // ...
    }
    
    ```
    
2.  Разработайте метод получения частот слов по id документа:
    
    ```cpp
    const map<string, double>& GetWordFrequencies(int document_id) const;
    
    ```
    
    Если документа не существует, возвратите ссылку на пустой  `map`.
    
3.  Разработайте метод удаления документов из поискового сервера
    
    ```cpp
    void RemoveDocument(int document_id);
    
    ```
    
4.  Вне класса сервера разработайте функцию поиска и удаления дубликатов:
    
    ```cpp
    void RemoveDuplicates(SearchServer& search_server);
    
    ```
    
    Дубликатами считаются документы, у которых наборы встречающихся слов совпадают. Совпадение частот необязательно. Порядок слов неважен, а стоп-слова игнорируются. Функция должна использовать только доступные к этому моменту методы поискового сервера.
    
    При обнаружении дублирующихся документов функция должна удалить документ с большим id из поискового сервера, и при этом сообщить id удалённого документа в соответствии с форматом выходных данных, приведённым ниже.
    

Будьте аккуратны, если функция  `RemoveDuplicates`  проходит циклом по поисковому серверу так:

```cpp
void RemoveDuplicates(SearchServer& search_server) {
    ...
    for (const int document_id : search_server) {
        ...
    }
    ...
}

```

В подобном случае удалять документы внутри цикла нельзя — это может привести к невалидности внутреннего итератора.

Все реализации должны быть эффективными. Если N — общее количество документов, а W — количество слов во всех документах, то:

-   сложность  `GetWordFrequencies`  должна быть O(\log N);
-   сложность  `RemoveDocument`  должна быть O(w(\log N+\log W)), где w — количество слов в удаляемом документе;
-   сложность  `begin`  и  `end`  — O(1);
-   сложность  `RemoveDuplicates`  должна быть O(wN(\log N+\log W)), где w — максимальное количество слов в документе.

В этом задании может потребоваться рефакторинг вашего кода. Например, замена одного вида контейнера на другой или введение нового индекса.

Вам предстоит оценить сложность разрабатываемых алгоритмов, чтобы знать, что они достаточно быстрые.

### Формат выходных данных

Функция  `RemoveDuplicates`  должна для каждого удаляемого документа вывести в  `cout`  сообщение в формате  `Found duplicate document id N`, где вместо  `N`  следует подставить id удаляемого документа.

### Ограничения

Сохраните корректную и быструю работу всех методов кроме  `GetDocumentId`.

### Что отправлять на проверку

Загрузите полный код поискового сервера и вспомогательных функций. Код нужно разбить на файлы. Функция  `main`  при проверке будет игнорироваться.

### Как будет тестироваться ваш код

Будет проверено, что:

-   вы не изменили работу методов и функций, которые не описаны в условии,
-   вы реализовали требуемые методы и функцию  `RemoveDuplicates`,
-   реализованные методы работают достаточно быстро и их сложность соответствует условию.

### Пример использования

```cpp
int main() {
    SearchServer search_server("and with"s);

    AddDocument(search_server, 1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7, 2, 7});
    AddDocument(search_server, 2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // дубликат документа 2, будет удалён
    AddDocument(search_server, 3, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // отличие только в стоп-словах, считаем дубликатом
    AddDocument(search_server, 4, "funny pet and curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // множество слов такое же, считаем дубликатом документа 1
    AddDocument(search_server, 5, "funny funny pet and nasty nasty rat"s, DocumentStatus::ACTUAL, {1, 2});

    // добавились новые слова, дубликатом не является
    AddDocument(search_server, 6, "funny pet and not very nasty rat"s, DocumentStatus::ACTUAL, {1, 2});

    // множество слов такое же, как в id 6, несмотря на другой порядок, считаем дубликатом
    AddDocument(search_server, 7, "very nasty rat and not very funny pet"s, DocumentStatus::ACTUAL, {1, 2});

    // есть не все слова, не является дубликатом
    AddDocument(search_server, 8, "pet with rat and rat and rat"s, DocumentStatus::ACTUAL, {1, 2});

    // слова из разных документов, не является дубликатом
    AddDocument(search_server, 9, "nasty rat with curly hair"s, DocumentStatus::ACTUAL, {1, 2});
    
    cout << "Before duplicates removed: "s << search_server.GetDocumentCount() << endl;
    RemoveDuplicates(search_server);
    cout << "After duplicates removed: "s << search_server.GetDocumentCount() << endl;
}

```

Ожидаемый вывод этой программы:

```cpp
Before duplicates removed: 9
Found duplicate document id 3
Found duplicate document id 4
Found duplicate document id 5
Found duplicate document id 7
After duplicates removed: 5

```

### Подсказка

Все подсказки есть в условии. Вы справитесь!

</details>

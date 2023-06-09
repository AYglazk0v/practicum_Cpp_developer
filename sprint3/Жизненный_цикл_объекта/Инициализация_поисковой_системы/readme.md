<details>  
<summary>Теория:</summary>

# Инициализация поисковой системы

Вы уже достаточно знаете о конструкторах, чтобы использовать их для улучшения поисковой системы.

Эффективность поиска во многом зависит от фильтрации стоп-слов. Эти слова не участвуют в индексации документов, добавляемых  `AddDocument`. Задать список стоп-слов можно методом  `SetStopWords`. Вызов  `SetStopWords`  никак не влияет на уже имеющиеся документы — стоп-слова учитываются только при добавлении следующих. Такие скрытые зависимости от порядка вызова методов — результат неудачного архитектурного решения. Избегайте их. Пользователям класса неожиданности не нужны.

Правильный сценарий использования класса  `SearchServer`  подразумевает установку списка стоп-слов до добавления документов. Это можно указать в комментариях к методам класса, но есть способ лучше. Установка списка стоп-слов — по сути, часть инициализации поисковой системы. Поэтому правильнее задать стоп-слова в конструкторе.

Когда явно заданных конструкторов нет, структуру  `Document`, несущую информацию о найденном документе, можем инициализировать, не перечисляя все поля. Но то, что это допускается, не значит, что делать так стóит:

```cpp
struct Document {
    int id;
    double relevance;
    int rating;
};

int main() {
    Document doc{10, 3.5}; // Поле rating будет проинициализировано 0
}


```

Если у структуры есть параметризованный конструктор, при её конструировании нужно указать значения всех параметров явно. Это пригодится, когда будете добавлять в структуру дополнительные поля, а в её параметризованный конструктор — дополнительные параметры. Код, передающий неполный набор параметров конструктору  `Document`, не скомпилируется, и вы легко его найдёте и исправите.

</details>  


<details>  
<summary>Задание:</summary>

## Задание

Замените способ настройки стоп-слов в классе  `SearchServer`. Вместо метода SetStopWords разработайте параметризованные конструкторы, принимающие стоп-слова в следующих форматах:

-   В виде строки, где стоп-слова разделены пробелами. В начале, в конце строки и между стоп-словами может быть произвольное количество пробелов.
-   В виде произвольной коллекции строк, такой как  `vector`  или  `set`. Пустые строки и слова-дубликаты внутри коллекции должны игнорироваться. Чтобы не дублировать код, примените шаблоны.

Пример инициализации класса  `SearchServer`:

```cpp
// Инициализируем поисковую систему, передавая стоп-слова в контейнере vector
const vector<string> stop_words_vector = {"и"s, "в"s, "на"s, ""s, "в"s};
SearchServer search_server1(stop_words_vector);

// Инициализируем поисковую систему передавая стоп-слова в контейнере set
const set<string> stop_words_set = {"и"s, "в"s, "на"s};
SearchServer search_server2(stop_words_set);

// Инициализируем поисковую систему строкой со стоп-словами, разделёнными пробелами
SearchServer search_server3("  и  в на   "s);

```

В структуре  `Document`  реализуйте:

-   конструктор по умолчанию, который инициализирует поля структуры нулевыми значениями;
-   параметризованный конструктор, позволяющий задать значения id документа, релевантности и рейтинга.

### Подсказка

Конструктор, как и обычная функция, бывает шаблонным. Шаблонный конструктор может принимать коллекцию стоп-слов, находящихся в разных коллекциях — в вашем случае, в  `vector`  и  `set`:

```cpp
class SearchServer {
public:
    template <typename StringCollection>
    explicit SearchServer(const StringCollection& stop_words) {
        ...
    }
    ...
}; 

```

Доработайте функцию  `SplitIntoWords`: исключите из неё возврат пустых слов. Реализуйте параметризованный конструктор, который принимает строку со стоп-словами.

</details>  

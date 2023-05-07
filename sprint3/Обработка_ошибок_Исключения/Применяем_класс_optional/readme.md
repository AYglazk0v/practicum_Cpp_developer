
<details>
<summary>Теория:</summary>

# Применяем класс optional

В прошлом уроке вы успели почувствовать неудобства, связанные с возвратом значений через выходные параметры функций:

-   Нужно заранее объявлять переменную-приёмник. Она передаётся в функцию по ссылке, куда функция записывает результат своей работы;
-   Нужно инициализировать выходной параметр внутри функции;
-   Нельзя использовать константы в качестве выходных параметров функций.

Вот был бы способ вместо выходного параметра вернуть либо результат функции, либо некоторое значение, которое означало бы ошибку! Такой способ есть, и вы его освоите прямо сейчас.

Возьмём решение, когда функция может либо вернуть некоторый результат, либо не вернуть его — например, из-за ошибки. Чтобы улучшить решение, применим встроенный шаблонный класс  `optional`. Он управляет  **опциональными значениями**  — значениями, которые могут быть представлены или не представлены. С ними ваша программа способна работать с переменными, которые в один момент времени хранят значение, а в другой — остаются пустыми.

Опциональные значения могут быть простой альтернативой кодам возврата, хотя их область применения этим не ограничивается. Как и в случае с классом  `vector`, тип значения, которое хранится в  `optional`, нужно указать внутри угловых скобок:

```cpp
// Функция возвращает пару корней квадратного уравнения либо пустое значение, когда решения нет
optional<pair<double, double>> SolveQuadraticEquation(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        // Возвращаем специальное значение nullopt, означающее отсутствие значения
        return nullopt;
    }

    double x1 = (-b - sqrt(discriminant)) / (2 * a);
    double x2 = (-b + sqrt(discriminant)) / (2 * a);

    // Возвращаем корни уравнения
    return pair{x1, x2};
}

```

Проверить экземпляр  `optional`  на наличие в нём значения можно, используя метод  `has_value`. Ещё вы можете обращаться с  `optional`  как со значением типа  `bool`, например в операторах ветвления. Пустой объект  `optional`  будет вести себя как  `false`, а непустой — как  `true`:

```cpp
optional<int> result;
...
if (result.has_value()) // либо просто: if (result)
{
    // result содержит значение
} else {
    // в переменной result пусто
}

```

Чтобы получить доступ к значению, хранящемуся внутри  `optional`, используют метод  `value`  и унарный оператор  `*`. Если внутри  `optional`  хранится структура или класс, можно получить доступ к его полям напрямую. Для этого есть специальный оператор  `->`:

```cpp
int main() {
    cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a, b, c;
    cin >> a >> b >> c;
    // Вместо const optional<pair<double, double>> roots используем auto,
    //     позволяя компилятору вывести тип переменной roots самостоятельно
    if (const auto roots = SolveQuadraticEquation(a, b, c);
        roots.has_value())
    {
        cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
        // Доступ к значению можно получить при помощи метода roots.value() или (*roots)
        // Если внутри хранится структура или класс, то доступ к его полям 
        //   можно получить при помощи ->
        cout << "  x1="s << roots.value().first << "; x2="s << roots->second << endl;
    } else {
        cout << "Уравнение не имеет действительных корней"s << endl;
    }
}

```

Переменную  `roots`  мы объявили прямо внутри условного выражения оператора  `if`, а после  `;`  проверили её значение. Такой способ объявления переменной позволяет ограничить её область видимости границами оператора  `if`, если за пределами этого оператора переменная не используется:

```cpp
if (string name = ReadString(); !name.empty()) {
    cout << "Привет, "s << name << endl;
} else {
    cout << "Привет, незнакомец"s << endl;
}

```

Для типа  `optional`  перегружена операция приведения к типу  `bool`. Поэтому проверить наличие значения можно просто:

```cpp
// Значение roots будет проверено на наличие значения 
// при помощи оператора приведения к типу bool
if (const auto roots = SolveQuadraticEquation(a, b, c)) {
    cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
    cout << "  x1="s << roots.value().first << "; x2="s << roots->second << endl;
} else {
    cout << "Уравнение не имеет действительных корней"s << endl;
}

```

Применив  `optional`, мы смогли написать функцию, которая способна сообщить об ошибке в ходе работы. При этом дополнительные выходные параметры в сигнатуру функции вводить не надо. Ключевое слово  `auto`  избавляет от многословности при объявлении типа переменной.

</details>

<details>
<summary>Задание:</summary>

## Задание

Упростите код поисковой системы, используя класс  `optional`  для возврата значений.

Для этого обновите сигнатуру методов  `MatchDocument`  и  `FindTopDocuments`, чтобы они возвращали результаты в виде опционального значения. В случае ошибки эти методы должны возвращать значение  `nullopt`, а результаты их работы не должны выводиться в стандартный поток вывода. Метод  `AddDocument`  по-прежнему должен возвращать значение типа  `bool`:  `true`  в случае успеха и  `false`  в случае неудачи:

```cpp
class SearchServer {
public:
    ...
    bool AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        ...
    }

    template <typename DocumentPredicate>
    optional<vector<Document>> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        ...
    }

    optional<vector<Document>> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        ...
    }

    optional<vector<Document>> FindTopDocuments(const string& raw_query) const {
        ...
    }

    optional<tuple<vector<string>, DocumentStatus>> MatchDocument(const string& raw_query, int document_id) const {
        ...
    }
    ...
};

```

Пример использования класса поисковой системы с обновлённым интерфейсом:

```cpp
void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    SearchServer search_server("и в на"s);

    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void) search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    if (const auto documents = search_server.FindTopDocuments("--пушистый"s)) {
        for (const Document& document : *documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}

```

### Подсказка

Внесите изменения в методы парсинга поискового запроса и документа, чтобы методы могли возвратом опциональных значений сообщить вызываемому коду, что есть ошибки.

</details>

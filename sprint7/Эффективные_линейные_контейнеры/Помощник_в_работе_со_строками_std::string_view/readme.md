<details>
<summary>Теория:</summary>

# Помощник в работе со строками: std::string_view

Строки похожи на вектор. Их элементы расположены последовательно в памяти. Они поддерживают доступ к элементам по номеру позиции. Разница в том, что строки содержат элементы только символьного типа. Но иногда мы ждем от строк большего, чем от вектора. Обычно строки содержат слова, и будет удобно уметь работать со словами, не создавая из строки новые строки, как мы делали это раньше.

### Ускоряем разделение строки на слова

Вспомним пример из поискового сервера:

```cpp
vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                // обратите внимание на это место в коде
                // здесь мы каждый раз копируем найденное слово
                // в новый элемент в векторе
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

```

Этот код можно переписать, используя итераторы и алгоритм  `find`. Но если посмотреть внимательно, код замедляется не из-за того, как именно мы итерируемся по строке. Каждый раз при добавлении элемента в вектор  `words`  создаётся новая строка — копия слова из  `text`. Это не самый быстрый метод, тем более мы просто хотим знать, где у слов начало и конец. Нам нужны не новые строки, а только позиции в уже имеющейся строке. Для этих целей и создан  `std::string_view`. Разберёмся с нашей функцией:

```cpp
// изменим название функции
// предыдущая её версия нам ещё пригодится

// пусть теперь наша функция возвращает вектор элементов string_view
vector<string_view> SplitIntoWordsView(const string& str) {
    vector<string_view> result;
    // 1
    int64_t pos = 0;
    // 2
    const int64_t pos_end = str.npos;
    // 3
    while (true) {
        // 4
        int64_t space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        if (space == pos_end) {
            break;
        } else {
            pos = space + 1;
        }
    }

    return result;
}

```

По сути,  `string_view`  — указатель на начало некой строки и её длина. Это не контейнер в прямом смысле: он не содержит элементы, а просто указывает на некую последовательность символов в памяти.

Новая функция вернёт вектор из  `string_view`, то есть вектор указателей, на начало слов и их дли́ны. Как раз то, что нам и было надо — избежим копирования слов в вектор.

Рассмотрим реализацию по пунктам. Следите по комментариям к функции:

1.  Создаём переменную, где будем сохранять начальную позицию для поиска следующего пробела. Иначе говоря, начало слова.
2.  Чтобы остановить поиск, нужен аналог итератора на конец. У  `string_view`  такую роль выполняет  `npos`  — специальная константа класса. Внутри это просто большое число, которое вряд ли когда-нибудь сможет оказаться реальной позицией в строке.
3.  Используем бесконечный цикл. Выходим из него, применяя  `break`, когда дойдём до конца строки.
4.  В цикле ищем следующий пробел, вызывая метод  `find`, который вернёт расстояние до ближайшего пробела, или  `npos`, если пробел не найден.
5.  Метод  `substr`  для  `string_view`  возвращает  `string_view`, то есть новая строка не создаётся. Просто продолжаем с указателями на уже имеющуюся строку. Если пробел не найден, добавляем в вектор всё, начиная с  `pos`. Если найден, выделяем слово от  `pos`  длиной  `space - pos`.
6.  Достигнув конца строки, выходим из цикла. Иначе, пропускаем одну позицию — пробел — и начинаем следующую итерацию цикла.

Запустим бенчмарк, где вызовем старую и новую функцию для большой строки:

```cpp
// создадим очень длинную строку,
// состоящую из слов из ста 'a'
string GenerateText() {
    const int SIZE = 10000000;
    string text(SIZE, 'a');
    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }
    return text;
}

int main() {
    const string text = GenerateText();
    {
        LOG_DURATION("string");
        const auto words = SplitIntoWords(text);
        // выведем первое слово
        cout << words[0] << "\n";
    }
    {
        LOG_DURATION("string_view");
        const auto words = SplitIntoWordsView(text);
        // выведем первое слово
        cout << words[0] << "\n";
    }

    return 0;
}

```

Результат работы (с максимальной оптимизацией):

```cpp
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
string: 34 ms
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
string_view: 13 ms

```

Неплохо.

### Но есть подвох

Поправим так, чтобы строка, которую нужно обработать, стала короткой:

```cpp
int main() {
    const string text = "a b";
    {
        LOG_DURATION("string");
        const auto words = SplitIntoWords(text);
        cout << words[0] << "\n";
    }
    {
        LOG_DURATION("string_view");
        const auto words = SplitIntoWordsView(text);
        cout << words[0] << "\n";
    }

    return 0;
}

```

Запустим и удивимся:

```cpp
a
string: 0 ms
⍰
string_view: 0 ms

```

Там, где мы ожидали увидеть  `a`, внезапно вывелся неизвестный символ.

----------

Предположите, в каком месте кода что-нибудь могло пойти не так. Можете использовать отладчик:

```cpp
vector<string_view> SplitIntoWordsView(const string& str) {
    vector<string_view> result;
    // 1
    int64_t pos = 0;
    // 2
    const int64_t pos_end = str.npos;
    // 3
    while (true) {
        // 4
        int64_t space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        if (space == pos_end) {
            break;
        } else {
            pos = space + 1;
        }
    }

    return result;
}

```

-   В пункте 2: кажется,  `npos`  для короткой строки работает неправильно.
    
-   В пункте 4: мы неверно инициализировали переменную  `pos`, поэтому результат поиска оказывается за пределами строки.
    
-   В пункте 5: подождите-ка,  `substr`  для аргумента-строки возвращает новую строку. А куда мы её сохранили? Наш вектор ведь сохраняет  `string_view`. Наверное, здесь что-нибудь нечисто.
    
-   В пункте 6: мы делаем лишний шаг, когда количество символов в слове оказывается нечётным.
    

Вы близко подошли к пониманию, что такое временные объекты. В данном случае имеем дело именно с ними.  `substr`  конструирует собственную строку. Она потом превращается в элемент типа  `string_view`, чтобы быть записанной в вектор. Подробнее временные объекты разберём в следующей теме.

`string_view`  — это указатель на строчку, которую мы никуда не сохранили. Она появилась и исчезла, а мы положили в вектор указатель на что-то переставшее существовать.

Чтобы исправить ситуацию, заменим тип аргумента. Пусть пользователь передаёт  `string_view`  и сам заботится о том, чтобы строка не оказалась временной. Декларируя таким образом тип аргумента, говорим пользователю, что мы не в ответе за объект самой строки. Если строка исчезнет, все результаты работы функции будут инвалидированы:

```cpp
vector<string_view> SplitIntoWordsView(string_view str) {
        ...
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        ...
}

```

Теперь метод  `substr`  работает не со строкой, а с указателем на строку. Никаких временных строк не создаётся.

Есть ещё один неочевидный бонус в использовании такой сигнатуры. Компилятор умеет превращать строки в объекты типа  `string_view`, но сделать обратное превращение он не может. Это значит, что в функцию, ожидающую  `string_view`, можно передать и строку. Все преобразования будут выполнены автоматически. Но если функция ожидает аргумент-строку, передать туда  `string_view`  уже не удастся.

</details>

<details>
<summary>Задание:</summary>

# Ответы на задания

----------

Предположите, в каком месте кода что-нибудь могло пойти не так. Можете использовать отладчик:

```cpp
vector<string_view> SplitIntoWordsView(const string& str) {
    vector<string_view> result;
    // 1
    int64_t pos = 0;
    // 2
    const int64_t pos_end = str.npos;
    // 3
    while (true) {
        // 4
        int64_t space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        if (space == pos_end) {
            break;
        } else {
            pos = space + 1;
        }
    }

    return result;
}

```

-   **(-)**  В пункте 2: кажется,  `npos`  для короткой строки работает неправильно.
    
-   **(-)**  В пункте 4: мы неверно инициализировали переменную  `pos`, поэтому результат поиска оказывается за пределами строки.
    
-   **(+)**  В пункте 5: подождите-ка,  `substr`  для аргумента-строки возвращает новую строку. А куда мы её сохранили? Наш вектор ведь сохраняет  `string_view`. Наверное, здесь что-нибудь нечисто.
    
-   **(-)**  В пункте 6: мы делаем лишний шаг, когда количество символов в слове оказывается нечётным.

</details>

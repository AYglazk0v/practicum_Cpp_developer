<details>  
<summary>Теория:</summary>

# Юнит-тестирование класса

В предыдущем уроке вы изучили основы создания юнит-тестов, которые проверяют работу функции. В этом уроке рассмотрим более сложный случай — разработку и юнит-тестирование класса.

В качестве примера выберем программу, решающую задачу «Синонимы».

### Задача «Синонимы»

Разработайте программу, которая реализует работу со словарём синонимов. Со стандартного ввода программе поступают три вида запросов:

-   `ADD word1 word2`  — добавить пару слов-синонимов  `word1`  и  `word2`;
-   `COUNT word`  — вывести в стандартный вывод количество синонимов для слова “word”;
-   `CHECK word1 word2`  — вывести  `YES`, если слова  `word1`  и  `word2`  в данный момент будут синонимами. Если не будут — вывести  `NO`;
-   `EXIT`  — завершить работу.

Пример входных и выходных данных:

```
ADD program code
ADD code cipher
COUNT cipher
1
CHECK code program
YES
CHECK program cipher
NO
EXIT

```

Из примера видно, что отношение «‎синонимичности» в этой программе:

-   симметрично: если первое слово — синоним второго, то и второе — синоним первого.
-   нетранзитивно: если A — синоним B, а B — синоним C, то A не будет синонимом C, если их не объявить синонимами явно.

### Анализ условия задачи

Прежде чем писать код, проанализируем условие. Как и в случае с задачей «‎Выбор наименьшей строки», функциональность программы можно условно разделить на две части:

1.  Ввод и вывод данных. Этот код разбирает поступающие запросы и выводит результаты.
    
2.  Работа со словарём синонимов. Этот код реализует логику добавления синонимов, подсчёт их количества, а также проверяет, будут два слова синонимами или нет.
    

###

### Разработка каркаса программы

Сначала разработаем каркас программы, взаимодействующий с пользователем. Для этого подключим библиотеку  `<sstream>`. Классы этой библиотеки позволяют считывать данные из строк и выводить данные в строки. Класс  `istringstream`  — это строковый поток ввода, а класс  `ostringstream`  — строковый поток вывода. В коде ниже стандартный класс  [`istringstream`](https://en.cppreference.com/w/cpp/io/basic_istringstream)  упрощает разбор строки:

```cpp
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string line;

    // В случае ошибок чтения из cin произойдёт выход из цикла
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            // Обработка команды ADD word1 word2
        } else if (action == "COUNT"s) {
            // Обработка команды COUNT word
        } else if (action == "CHECK"s) {
            // Обработка команды CHECK word1 word2
        } else if (action == "EXIT"s) {
            break;
        }
    }
}

```

Класс  [`istringstream`](https://en.cppreference.com/w/cpp/io/basic_istringstream)  позволяет считывать данные из содержимого строки подобно тому, как объект  `cin`  делает это со стандартным потоком ввода. То есть сначала считываем строку функцией  `getline`, затем создаём новый строковый поток  `command`. Он нужен, чтобы разобрать содержимое считанной строки.

Пока единственная команда, которую программа умеет обрабатывать, —  `EXIT`. Но этого уже достаточно для проверки её работы.

На следующем шаге добавим разбор параметров остальных команд:

```cpp
int main() {
    string line;

    // В случае ошибок чтения из cin произойдёт выход из цикла
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;

            // TODO: добавить пару синонимов first_word и second_word

        } else if (action == "COUNT"s) {
            string word;
            command >> word;

            // TODO: вывести количество синонимов word

        } else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;

            // TODO: проверить, являются ли first_word и second_word синонимами
            // и вывести YES либо NO

        } else if (action == "EXIT"s) {
            break;
        }
    }
}

```

Реализуем логику обработки команд  `ADD`,  `COUNT`  и  `CHECK`. Для этого создадим класс  `Synonyms`. Он инкапсулирует данные, нужные для хранения базы синонимов, и предоставляет методы для выполнения основных операций над синонимами:

```cpp
class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
    }

    size_t GetSynonymCount(const string& word) const {
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        return false;
    }
};
```

Преимущества класса `Synonyms` над отдельными функциями, которые принимают коллекцию синонимов:

-   Класс прячет свои данные в приватной области. Поэтому мы можем изменять структуру данных для хранения синонимов, не влияя на код за пределами класса.
-   Упрощается код тестов: тесты проверяют наблюдаемое поведение класса, доступное благодаря его публичным методам, не привязываясь к деталям реализации.
-   Методы класса обеспечивают целостность данных, защищая их от случайной модификации извне.

Методы `GetSynonymCount` и `AreSynonyms` на этом этапе возвращают жёстко заданные значения для того, чтобы код просто скомпилировался без ошибок.

Подставим класс  `Synonyms`  в функцию  `main`:

```cpp
int main() {
    Synonyms synonyms;

    string line;
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        } else if (action == "COUNT"s) {
            string word;
            command >> word;
            cout << synonyms.GetSynonymCount(word) << endl;
        } else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            if (synonyms.AreSynonyms(first_word, second_word)) {
                cout << "YES"s << endl;
            } else {
                cout << "NO"s << endl;
            }
        } else if (action == "EXIT"s) {
            break;
        }
    }
} 
```

Убедимся, что программа компилируется без ошибок. Но теперь вместо того, чтобы приняться за реализацию методов класса `Synonyms`, напишем модульный тест ДО написания кода.

Написание теста перед написанием кода называется «‎разработка через тестирование»‎ (test-driven development, TDD). Этот подход заключается в следующем:

-   пишем небольшой тест, предъявляющий к тестируемому классу или функции новые требования;
-   запускаем тест, чтобы убедиться: модуль не проходит. Этот этап показывает, что тест работает и проверяет новый, ещё не реализованный функционал;
-   дорабатываем модуль так, чтобы успешно пройти новый тест и все ранее написанные;
-   пишем новый тест, и процесс начинается сначала.

Такой подход обеспечивает хорошее покрытие тестами, ведь мы создаём код, для которого уже написали «‎падающий»‎ тест. Когда написать тест, который модуль не смог бы пройти, невозможно, разработка останавливается. Тесты как бы ведут разработку за собой. Отсюда и название подхода.

Как и в примере из прошлого урока, тест будет выполнять некоторые операции над экземпляром класса, а затем при помощи макроса  `assert`  сравнивать состояния класса с ожидаемым.

Напишем первый тест и вызовем его в начале программы. Тест будет проверять, как работает функция добавления синонимов: после вызова метода `Add` должно увеличиться количество синонимов, возвращаемое методом `GetSynonymCount`:

```cpp
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

class Synonyms {
    ...
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    assert(synonyms.GetSynonymCount("music"s) == 0);
    assert(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    // Два слова являются синонимами друг друга
    assert(synonyms.GetSynonymCount("music"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    assert(synonyms.GetSynonymCount("music"s) == 2);

    // Отношение синонимичности в нашей постановке задачи не является транзитивным.
    // Поэтому слова "tune" и "melody" синонимами друг друга не являются,
    // несмотря на то что оба они являются синонимами слова "music".
    assert(synonyms.GetSynonymCount("tune"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
}

int main() {
    TestSynonyms();
    ...
}

```

Скомпилируем программу и увидим, что тесты не проходят, ведь нужные изменения в класс  `Synonyms`  мы ещё не внесли:

```
synonyms.cpp:29: void TestAddingSynonymsIncreasesTheirCount(): Assertion `synonyms.GetSynonymCount("music"s) == 1' failed.

```

Этот шаг позволяет удостовериться, что тесты вообще запускаются и предъявляют к нашей программе новые требования.

На следующем шаге внесём в тестируемый код изменения, достаточные для прохождения теста. Добавим в класс  `Synonyms`  поле  `synonyms_`  типа  `map`. Ключами будут слова, а значениями — множества их синонимов. Также внесём изменения в методы  `Add`  и  `GetSynonymCount`:

```cpp
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if (synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        return false;
    }

private:
    map<string, set<string>> synonyms_;
};

```

Скомпилируем и запустим программу:

```
synonyms.cpp:38: void TestAddingSynonymsIncreasesTheirCount(): Assertion `synonyms.GetSynonymCount("melody"s) == 1' failed.

```

Она сообщает, что одно из условий не выполняется:

```cpp
void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    assert(synonyms.GetSynonymCount("music"s) == 0);
    assert(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    assert(synonyms.GetSynonymCount("music"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);  // <--- это условие не выполнилось

    synonyms.Add("music"s, "tune"s);
    assert(synonyms.GetSynonymCount("music"s) == 2);
    assert(synonyms.GetSynonymCount("tune"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);
}
```

Так как количество слов первого синонима оказалось верным, а количество синонимов второго слова — неверным, предположим, что ошибка закралась в логику добавления синонимов. Находим ошибку в методе `Add`: второе слово добавляется в качестве синонима первого, а первое слово в качестве синонима второго нет. Исправляем ошибку:

```cpp
class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    ...
};
```

Снова скомпилируем и запустим программу. В этот раз тесты отработают успешно. Но это не значит, что программа готова. Мы ещё не реализовали и не протестировали метод `AreSynonyms`. Вы сделаете это самостоятельно в задании.

Юнит-тесты позволяют обнаружить и исправить ошибки на ранних этапах. Такой подход стимулирует декомпозицию программы на независимые функциональные модули — так их легче тестировать.
</details>  

<details>  
<summary>Задание:</summary>

<p>Реализуйте в программе «Синонимы» недостающий функционал метода <code>AreSynonyms</code> и проведите над ним модульное тестирование. Решение задания пригодится вам в следующей теме. Сохраните его в своей IDE или на GitHub.</p>

<h3>Подсказка</h3>

<p>Напишите тест, который проверяет, что слова, добавленные в качестве синонимов, действительно синонимы друг друга, а слова, которые не добавлены в качестве синонимов, — не синонимы. Запустите программу и убедитесь, что тест не проходит. Реализуйте недостающий функционал в методе <code>AreSynonyms</code>, чтобы тесты проходили. Затем проверьте работу программы вручную. Писать тест до реализации метода пока может казаться трудным. Ничего страшного. Просто реализуйте метод привычным вам способом, а затем напишите модульный тест для его проверки.</p>
</details>  

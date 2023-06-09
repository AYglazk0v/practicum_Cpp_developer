<details>  
<summary>Теория:</summary>

# Работаем с cerr

Когда юнит-тесты написаны, важно знать, что они выполняются. Для этого добавим в код вывод сообщения об успешном завершении очередного теста:

```cpp
void TestAddingSynonymsIncreasesTheirCount() {
    ...
    cout << "TestAddingSynonymsIncreasesTheirCount OK"s << endl;
}

void TestAreSynonyms() {
    ...
    cout << "TestAreSynonyms OK"s << endl;
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int main() {
    TestSynonyms();
    ...
}

```

Запустим программу и увидим: она вывела названия выполненных тестов и ожидает пользовательский ввод:

```
TestAddingSynonymsIncreasesTheirCount OK
TestAreSynonyms OK

```

Если отправим программу в таком виде в тестирующую систему, она не пройдёт автоматические проверки. Ведь вывод программы перемешался с выводом тестов, и выходные данные программы не совпадут с ожидаемыми. Можно всякий раз комментировать вывод названий тестов или убрать его совсем. Но есть способ лучше: мы можем доработать тесты так, чтобы диагностическая информация выводилась не в стандартный поток вывода, а в стандартный поток ошибок  `cerr`.

`cerr`  — один из трёх стандартных потоков. Он предназначен для вывода диагностической информации. Работа с  `cerr`  идентична работе с объектом  `cout`.

По умолчанию поток  `cerr`  связан с выводом в консоль, но во время проверки приложения тестирующая система будет игнорировать вывод в поток ошибок.

Чтобы выводить диагностические сообщения в поток ошибок, заменим  `cout`  на  `cerr`  в функциях  `AssertEqualImpl`  и  `AssertImpl`, а также в тестирующих функциях:

```cpp
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

```

```cpp
void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "Assert("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

```

```cpp
void TestAddingSynonymsIncreasesTheirCount() {
    ...

    cerr << "TestAddingSynonymsIncreasesTheirCount OK"s << endl;
}

void TestAreSynonyms() {
    ...

    cerr << "TestAreSynonyms OK"s << endl;
}

```

Теперь диагностическая информация будет выводиться в поток ошибок.

Посмотрите на код, который выводит названия успешно пройденных тестов. Сразу бросается в глаза дублирование. Во-первых, в каждом тесте приходится писать однотипный код. Во-вторых, надо следить, чтобы выводимое имя теста совпадало с именем тестирующей функции. В задании вы разработаете шаблонную функцию и макрос и исправите недостатки.
</details>  

<details>  
<summary>Задание:</summary>



## Задание

Разработайте шаблонную функцию  `RunTestImpl`  и использующий её макрос  `RUN_TEST`. Они должны запускать тесты и выводить сообщения об их завершении в стандартный поток ошибок.

### Прекод

```cpp
#include <iostream>
#include <string>

using namespace std;

template </*напишите недостающий код*/>
void RunTestImpl(/*Напишите недостающий код*/) {
    /* Напишите недостающий код */
}

#define RUN_TEST(func)  // напишите недостающий код

void Test1() {
}

int main() {
    RUN_TEST(Test1);
}

```

Если функция  `Test1`  выполняется успешно, в стандартный поток ошибок должно быть выведено:

```
Test1 OK

```

### Подсказка

Чтобы узнать имя функции, получите строковое представление параметра  `func`  так же, как получаете строковое значение аргументов в макросе  `ASSERT_EQUAL`. Передайте его в функцию  `RunTestImpl`  вторым параметром. В качестве первого параметра функция  `RunTestImpl`  должна принимать функцию-тест. Для этого функцию  `RunTestImpl`  сделайте шаблонной — компилятор сможет вывести тип самостоятельно. Чтобы проверить, как работает фреймворк, попробуйте внести логические ошибки в методы класса  `Synonyms`, например:

```cpp
bool AreSynonyms(const string& first_word, const string& second_word) const {
    if (synonyms_.count(first_word) != 0) {
        return synonyms_.at(first_word).count(second_word) != 0;
    }
    // Ошибка сделана специально и проверяет работу фреймворка
    return true; // следует возвращать false
}
```
</details>  

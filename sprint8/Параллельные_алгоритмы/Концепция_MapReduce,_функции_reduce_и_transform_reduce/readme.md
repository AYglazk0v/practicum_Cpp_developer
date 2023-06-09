<details>
<summary>Теория:</summary>

# Концепция MapReduce, функции reduce и transform_reduce

Алгоритм  `transform`  реализует первую стадию в концепции MapReduce. Для второй стадии есть  [алгоритм  `reduce`](https://en.cppreference.com/w/cpp/algorithm/reduce)  из библиотеки  `<numeric>`. Он принимает входной диапазон итераторов, начальное значение — по умолчанию ноль или пустая строка — и группирующую операцию — по умолчанию сумма.

Просуммируем элементы вектора  `marks`, подсчитаем пробелы, а затем и слова:

```cpp
#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string_view>

using namespace std;

int main() {
    const string_view s = "pretty little octopus"sv;
    vector<int> marks(s.size());
    transform(execution::par, s.begin(), s.end(), marks.begin(),
              [](char c) { return c == ' '; });
    cout << reduce(execution::par, marks.begin(), marks.end()) + 1
         << " words"s << endl;
    // вывод: 3 words
}

```

----------

Это же умеет функция  `accumulate`! Почему для неё не сделали параллельную версию, а придумали отдельный  `reduce`?

-   Функция  `accumulate`  обещает конкретный порядок применения операции, который параллельная версия  `reduce`  гарантировать не может.
    
-   Просто забыли. Так бывает: спешишь внедрить новую фичу и случайно изобретаешь велосипед.
    
-   Хотели единообразия названий: никто же не поймёт, что  `accumulate`  — это вторая стадия MapReduce.
    

[Функция  `transform_reduce`](https://en.cppreference.com/w/cpp/algorithm/transform_reduce)  позволяет обойтись без вектора для хранения результатов между стадиями. В неё, в отличие от  `reduce`, нужно обязательно передать операцию для группировки объектов. Но нет нужды писать лямбду. Подключите библиотеку  `<functional>`  и создайте функциональный объект  `plus<>{}`, который, будучи вызванным как функция, суммирует аргументы:

```cpp
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <string_view>

using namespace std;

int main() {
    const string_view s = "pretty little octopus"sv;
    const int space_count = transform_reduce(
        execution::par,  // для демонстрации, можно и убрать
        s.begin(), s.end(),  // входной диапазон
        0,  // начальное значение
        plus<>{},  // reduce-операция (группирующая функция)
        [](char c) { return c == ' '; }  // map-операция
    );
    cout << space_count + 1 << " words"s << endl;  // вывод: 3 words
}
```

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Это же умеет функция  `accumulate`! Почему для неё не сделали параллельную версию, а придумали отдельный  `reduce`?

-   **(+)**  Функция  `accumulate`  обещает конкретный порядок применения операции, который параллельная версия  `reduce`  гарантировать не может.
    
-   **(-)**  Просто забыли. Так бывает: спешишь внедрить новую фичу и случайно изобретаешь велосипед.
    

> Не стоит недооценивать комитет по стандартизации C++!

-   **(-)**  Хотели единообразия названий: никто же не поймёт, что  `accumulate`  — это вторая стадия MapReduce.

> Но  `transform`  же не стали переименовывать в  `map`.

</details>

<details>
<summary>Задание 1:</summary>

## Задание 1

И снова считаем слова. Напишите функцию  `int CountWords(string_view str)`, подсчитывающую количество слов в переданной строке.

Пример использования — в заготовке кода.

### Ограничения

Всё ещё гарантируется, что строка  `str`  содержит только латинские буквы и пробелы. Однако теперь ваша функция должна корректно вычислять количество слов для строк, начинающихся с пробела, заканчивающихся пробелом или содержащих несколько пробелов подряд.

В этой задаче нельзя использовать параллельные алгоритмы. Достаточной эффективности можно добиться простым применением концепции MapReduce.

Ваш код не должен содержать слов  `execution`  или  `thread`.

### Что отправлять на проверку

cpp-файл, содержащий функцию  `CountWords`, а также нужные для её работы подключения библиотек, другие классы и функции. Функция  `main`  будет проигнорирована при тестировании.

### Как будет тестироваться ваш код

Правильность работы функции будет проверена юнит-тестами.

Производительность вашей функции мы проверим, сравнив её по времени работы с функцией, приведённой в заготовке кода: ваша версия не должна проигрывать сильнее, чем в 1,5 раза. Мы проверим это на строке длиной 100 000 000 символов.

Обратите внимание, что в новых условиях на входные данные функция из заготовки кода не решает задачу корректно.

Вы можете ориентироваться на бенчмарк, данный в заготовке кода.

### Подсказка

Примените  `transform_reduce`  для вычисления количества пробелов, за которыми следует буква. Вам поможет версия, принимающая два входных диапазона.

</details>

<details>
<summary>Задание 2:</summary>

## Задание 2

Используйте  `<execution>`, чтобы ускорить решение предыдущей задачи.

### Ограничения

Разрешено подключать  `<execution>`  и использовать параллельные версии алгоритмов.

Для строки длиной 50 000 000 символов время работы вашей функции должно быть по крайней мере вдвое меньше, чем у авторского последовательного решения предыдущей задачи.

### Подсказка

`execution::par`  и  `transform_reduce`  — отличная пара.

</details>

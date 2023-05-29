<details>
<summary>Теория:</summary>

# О размере критической области

С критическими областями — и, соответственно, мьютексами — нужно быть внимательными. Если вся функциональность потока будет большой единой критической областью, никакой параллельности не возникнет. Скорее всего, вы даже увидите замедление программы из-за дополнительных затрат на синхронизацию процессов.

В качестве примера рассмотрим функцию, которая вычисляет частоты всех чисел из данного вектора, и тестирующий её бенчмарк:

```cpp
#include "log_duration.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <string_view>
#include <vector>

using namespace std;

// максимальная величина числа
// constexpr означает, что эта переменная вычисляется на этапе компиляции
constexpr int MAX_VALUE = 1000;

// первая версия — с гонкой
vector<int> CountFrequenciesForEachRace(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs](int number) { ++freqs[number]; }
    );
    return freqs;
}

// случайные count чисел из диапазона [0, MAX_VALUE]
vector<int> GenerateNumbers(mt19937& generator, int count, int max_value = MAX_VALUE) {
    vector<int> numbers(count);
    for (int& number : numbers) {
        number = uniform_int_distribution(0, max_value)(generator);
    }
    return numbers;
}

// тестирует время работы и выводит сумму всех частот
template <typename Function>
void Test(string_view mark, const vector<int>& numbers, Function function) {
    vector<int> freqs;
    {
        LOG_DURATION(mark);
        freqs = function(numbers);
    }
    cout << "sum = " << reduce(freqs.begin(), freqs.end()) << endl;
}

#define TEST(function) Test(#function, numbers, function)

int main() {
    mt19937 generator;
    const auto numbers = GenerateNumbers(generator, 100'000'000);

    TEST(CountFrequenciesForEachRace);
}

```

Функция  `CountFrequenciesForEachRace`  вычисляет частоты параллельно, но с гонкой: несколько потоков могут одновременно увеличивать один и тот же элемент вектора  `freqs`. При запуске эта программа может вывести следующее:

```
CountFrequenciesForEachRace: 365 ms
sum = 93200471

```

Действительно, сумма частот оказалась неравна количеству чисел — 100 000 000. Чтобы избежать гонки при изменении вектора  `freqs`, можно защитить его одним мьютексом:

```cpp
#include <mutex>

vector<int> CountFrequenciesForEachOneMutex(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    mutex freqs_mutex;
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs, &freqs_mutex](int number) {
            lock_guard guard(freqs_mutex);
            ++freqs[number];
        }
    );
    return freqs;
}

```

Добавим в  `main`  строчку  `TEST(CountFrequenciesForEachOneMutex);`  и запустим программу. Результат будет корректным, но слишком медленным — вспоминаем про накладные расходы:

```
CountFrequenciesForEachRace: 364 ms
sum = 93177029
CountFrequenciesForEachOneMutex: 11732 ms
sum = 100000000

```

Мы запретили нескольким потокам одновременно менять вектор  `freqs`. Но нет ничего страшного в том, чтобы одновременно менять разные элементы. Поэтому гораздо эффективнее контролировать доступ к отдельным элементам с помощью вектора мьютексов:

```cpp
vector<int> CountFrequenciesForEachSeparateMutexes(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    vector<mutex> freqs_mutexes(MAX_VALUE + 1);
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs, &freqs_mutexes](int number) {
            lock_guard guard(freqs_mutexes[number]);
            ++freqs[number];
        }
    );
    return freqs;
}

```

Добавим в  `main`  строчку  `TEST(CountFrequenciesForEachSeparateMutexes);`  и запустим программу:

```
CountFrequenciesForEachRace: 364 ms
sum = 93186382
CountFrequenciesForEachOneMutex: 12966 ms
sum = 100000000
CountFrequenciesForEachSeparateMutexes: 1070 ms
sum = 100000000

```

Стало на порядок быстрее, но всё ещё медленнее варианта с гонкой. Зато результат корректный.

</details>

<details>
<summary>Задание:</summary>

## Задание

Как вы, возможно, помните, алгоритм  `copy_if`  фильтрует элементы из данного диапазона и сохраняет подходящие в указанный выходной итератор, сохраняя их исходный порядок. Ваша задача — ускорить  `copy_if`  за счёт параллельности и отказа от требования сохранения порядка элементов.

Напишите шаблонную функцию  `CopyIfUnordered`, которая:

-   принимает контейнер элементов и предикат — функцию, принимающую элемент и возвращающую  `bool`;
-   возвращает вектор элементов, для которых предикат вернул  `true`;
-   порядок элементов в итоговом векторе не имеет значения, но каждый элемент, удовлетворяющий предикату, должен присутствовать в результате ровно столько же раз, сколько в исходном контейнере.

В заготовке кода вам дана последовательная версия этой функции. Ускорьте её.

### Ограничения

Гарантируется, что для типа  `Container`  можно получить тип хранящихся в нём объектов, используя выражение  `typename Container::value_type`. Эти элементы имеют конструктор по умолчанию и конструктор копирования.

### Что отправлять на проверку

cpp-файл, содержащий шаблонную функцию  `CopyIfUnordered`, а также нужные для её работы подключения библиотек и другие функции. Функция  `main`  будет проигнорирована при тестировании.

### Как будет тестироваться ваш код

Помимо корректности будет протестирована скорость работы функции.

Ваша функция должна быть эффективнее последовательного решения из заготовки кода минимум на 40% при следующих условиях:

-   контейнер — вектор и содержит 50 000 строк длиной до 3 000 символов;
-   фильтрующий предикат имеет линейную по длине строки сложность.

Вы можете ориентироваться на бенчмарк, данный в заготовке кода.

### Подсказка

Минимизируйте критическую область вокруг вставки в конец вектора. Вынесите из неё не только проверку условия, но и копирование нового элемента.

</details>

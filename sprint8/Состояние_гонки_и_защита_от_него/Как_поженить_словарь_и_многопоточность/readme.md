<details>
<summary>Теория:</summary>

# Как поженить словарь и многопоточность

Мы успешно поускоряли функцию  `CountFrequencies`  — во многом благодаря тому, что значения можно было использовать в качестве индексов небольшого вектора. Если же разброс значений будет большим, частоты придётся хранить в словаре.

Протестируем версии без мьютексов — с ато́миками и без:

```cpp
#include "log_duration.h"

#include <atomic>
#include <execution>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

constexpr int MAX_VALUE = 1'000'000'000;

map<int, int> CountFrequenciesForEachRace(const vector<int>& numbers) {
    map<int, int> freqs;
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs](int number) { ++freqs[number]; }
    );
    return freqs;
}

map<int, int> CountFrequenciesForEachAtomicRace(const vector<int>& numbers) {
    map<int, atomic_int> freqs;
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs](int number) { ++freqs[number]; }
    );
    return {freqs.begin(), freqs.end()};
}

vector<int> GenerateNumbers(mt19937& generator, int count, int max_value = MAX_VALUE) {
    vector<int> numbers(count);
    for (int& number : numbers) {
        number = uniform_int_distribution(0, max_value / 1'000'000)(generator) * 1'000'000;
    }
    return numbers;
}

template <typename Function>
void Test(string_view mark, const vector<int>& numbers, Function function) {
    map<int, int> freqs;
    {
        LOG_DURATION(mark);
        freqs = function(numbers);
    }
    const int sum = transform_reduce(
        freqs.begin(), freqs.end(),
        0,
        plus<>{},
        [](const auto& item) { return item.second; }
    );
    cout << "sum = " << sum << endl;
}

#define TEST(function) Test(#function, numbers, function)

int main() {
    mt19937 generator;
    const auto numbers = GenerateNumbers(generator, 1'000'000);

    TEST(CountFrequenciesForEachRace);
    TEST(CountFrequenciesForEachAtomicRace);
}

```

В обоих случаях программа упадёт: при отсутствии ключа он будет добавляться, и делать со словарём что-либо параллельно этому небезопасно.

Используем мьютекс:

```cpp
#include <mutex>

map<int, int> CountFrequenciesMutex(const vector<int>& numbers) {
    map<int, int> freqs;
    mutex freqs_mutex;
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs, &freqs_mutex](int number) {
            lock_guard g(freqs_mutex);
            ++freqs[number];
        }
    );
    return freqs;
}

```

Добавление в словарь — непростая операция, которая может изменить всю его структуру. Поэтому не получится увеличить количество мьютексов и распараллелить какие-либо добавления. Нужно что-то делать со словарём.

Представим, что в словаре хранятся все ключи от 1 до 10000. Интуитивно кажется: когда из одного потока обращаетесь к ключу 10, а из другого — например, к ключу 6712, нет смысла защищать эти обращения одним и тем же мьютексом. Это отдельные области памяти, а внутреннюю структуру словаря мы никак не изменяем. А если будем обращаться к ключу 6712 одновременно из нескольких потоков, синхронизация, несомненно, понадобится.

Отсюда возникает идея: разбить словарь на несколько подсловарей с непересекающимся набором ключей и защитить каждый из них отдельным мьютексом. Тогда при обращении разных потоков к разным ключам они нечасто будут попадать в один и тот же подсловарь, а значит, смогут параллельно его обрабатывать. Эту идею вам предстоит реализовать в задании.

</details>

<details>
<summary>Задание:</summary>

## Задание

Напишите шаблонный класс `ConcurrentMap` со следующим интерфейсом:

```cpp
template <typename Key, typename Value>
class ConcurrentMap {
public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys"s);

    struct Access {
        Value& ref_to_value;

        // ...
    };

    explicit ConcurrentMap(size_t bucket_count);

    Access operator[](const Key& key);

    std::map<Key, Value> BuildOrdinaryMap();

private:
    // ...
};

```

-   `static_assert` в начале класса не даст программе скомпилироваться при попытке использовать в качестве типа ключа что-либо, кроме целых чисел.
-   Конструктор класса `ConcurrentMap<Key, Value>` принимает количество подсловарей, на которые надо разбить всё пространство ключей.
-   `operator[]` должен вести себя так же, как аналогичный оператор у `map`: если ключ `key` есть в словаре, должен возвращаться объект класса `Access`, содержащий ссылку на соответствующее ему значение. Если `key`  в словаре нет, в него надо добавить пару `(key, Value())` и вернуть объект класса `Access`, содержащий ссылку на только что добавленное значение.
-   Структура `Access`  должна вести себя так же, как в шаблоне `Synchronized`: предоставлять ссылку на значение словаря и обеспечивать синхронизацию доступа к нему.
-   Метод `BuildOrdinaryMap` должен сливать вместе части словаря и возвращать весь словарь целиком. При этом он должен быть потокобезопасным, то есть корректно работать, когда другие потоки выполняют операции с `ConcurrentMap`.

В заготовке кода используются тесты на основе фреймворка  `test_framework.h`, с которым вы уже знакомы. Для проверки решения на вашем компьютере загрузите этот фреймворк по ссылке из урока «mutex и lock_guard».

### Ограничения

Ваш класс должен гарантировать потокобезопасную работу со словарём, как описано в условии. При этом он должен быть как минимум вдвое эффективнее обычного словаря с общим мьютексом.

Гарантируется, что тип  `Value`  имеет конструктор по умолчанию и конструктор копирования.

### Что отправлять на проверку

cpp-файл, содержащий шаблонный класс  `ConcurrentMap`, а также нужные для его работы подключения библиотек и другие функции. Ваша программа не должна содержать подстроку  `copy_if`. Функция  `main`  будет проигнорирована при тестировании.

### Как будет тестироваться ваш код

Пример тестов и бенчмарка вы найдёте в заготовке кода.

### Подсказка

Ключи — целые числа. Так что операция взятия остатка от деления поможет вам выбрать, в подсловаре с каким индексом хранить данный ключ. Чтобы не мучиться с отрицательными числами, предварительно приведите ключ к типу  `uint64_t`.

</details>

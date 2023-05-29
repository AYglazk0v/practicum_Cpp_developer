<details>
<summary>Теория:</summary>

# atomic-типы

Мьютексы — универсальное и удобное средство. Но именно из-за своей гибкости они могут быть недостаточно эффективны.

В примере с подсчётом частот мьютексы использовались лишь для того, чтобы инкремент одной и той же переменной не выполнился одновременно:

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

Вот было бы здорово, если бы существовали особые переменные, которые сами бы разруливали одновременные операции с собой, не позволяя им конфликтовать. И они есть. Это атомарные переменные.

Библиотека  `<atomic>`  содержит шаблонный класс  `atomic`. В него можно обернуть любой числовой тип:  `bool`, целое или вещественное число и даже указатель:  `atomic<int>`  — это атомарное целое число,  `atomic<string*>`  — атомарный указатель на строчку. Есть и удобные синонимы: например,  `atomic_int`  — это  `atomic<int>`,  `atomic_flag`  — это  `atomic<bool>`, но со специальными методами.

Сделаем все счётчики частот атомарными и посмотрим, что это даст:

```cpp
#include <atomic>

vector<int> CountFrequenciesForEachAtomic(const vector<int>& numbers) {
    vector<atomic_int> freqs(MAX_VALUE + 1);
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs](int number) { ++freqs[number]; }
    );
    return {freqs.begin(), freqs.end()};
}

```

Два интересных момента:

1.  Теперь можно спокойно вызывать  `++freqs[number]`. Операции  `freqs[number]`  — получение ссылки на элемент по его индексу — могут выполняться одновременно. И это хорошо, потому что операции чтения можно параллелить. Зато инкремент — это операция с атомарной целочисленной переменной и потому она сама будет атомарной, то есть во время её выполнения другие операции с этой переменной выполняться не будут.
2.  `vector<atomic_int>`  и  `vector<int>`  — разные типы, поэтому возвращаемый вектор строится по вектору атомарных целых чисел с помощью конструктора по двум итераторам.

Если добавить в  `main`  строчку  `TEST(CountFrequenciesForEachAtomic);`, вы увидите, что код стал эффективнее. Но некорректный вариант с гонкой всё равно выигрывает:

```
CountFrequenciesForEachRace: 364 ms
sum = 93192508
CountFrequenciesForEachOneMutex: 12710 ms
sum = 100000000
CountFrequenciesForEachSeparateMutexes: 1070 ms
sum = 100000000
**CountFrequenciesForEachAtomic: 610 ms
sum = 100000000**

```

Попробуем теперь применить атомарные типы к классу  `Account`. Уберём мьютекс и сделаем остаток на счету атомарным:

```cpp
class Account {
public:
    explicit Account(int value) : value_(value) {}
    int GetValue() const { return value_; }
    bool TrySpend(int plan) {
        if (value_ < plan) {
            return false;
        } else {
            value_ -= plan;
            return true;
        }
    }
private:
    atomic_int value_;
};

```

----------

Что можно сказать о работе такой программы? Выберите несколько вариантов ответа.

-   Перетрат не будет, и баланс не уйдёт в минус.
    
-   Баланс может уйти в минус.
    
-   Как и раньше, сумма трат каждого человека может оказаться больше суммарного изменения переменной  `value_`.
    

Сама по себе операция  `value_ < plan`  не атомарна. Атомарно лишь чтение значения  `value_`. А вот вычитание  `value_ -= plan`  выполняется атомарно, что, впрочем, спасает не от всех проблем.

Пример вывода:

```
Spent 323937
Spent 355618
Spent 172041
Spent 149755
-1351

```

Можете убедиться, что 323937 + 355618 + 172041 + 149755 = 1000000 + 1351. То есть баланс ушёл в минус, но хотя бы все траты учтены верно.

Можно переписать метод  `TrySpend`  так:

```cpp
bool TrySpend(int plan) {
    if ((value_ -= plan) < 0) {
        value_ += plan;
        return false;
    } else {
        return true;
    }
}

```

Тогда перетрат не будет. Но некоторые вызовы могут зря завершиться неудачей, потому что начнут работать с  `value_`  в тот момент, когда он будет отрицательным и не вернётся временно перетратившим процессом к прежнему значению.

К сожалению, для  `atomic_int`  нет атомарной операции «вычти  `x`, если после этого число останется неотрицательным».

Но если все тратят строго по одному рублю, задача упрощается:

```cpp
#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <random>
#include <vector>

using namespace std;

class Account {
public:
    explicit Account(int value) : value_(value) {}
    int GetValue() const { return value_; }
    bool TrySpendOne() {
        if (value_.fetch_sub(1) <= 0) {
            ++value_;
            return false;
        } else {
            return true;
        }
    }
private:
    atomic_int value_;
};

int SpendAll(Account& account, int plans) {
    int total_spent = 0;
    for (int i = 0; i < plans; ++i) {
        if (account.TrySpendOne()) {
            ++total_spent;
        } else {
            break;
        }
    }
    return total_spent;
}

int GeneratePlans(mt19937& generator, int max_count = 1'000'000) {
    return uniform_int_distribution(0, max_count)(generator);
}

int main() {
    mt19937 generator;

    const int mother_plans   = GeneratePlans(generator);
    const int father_plans   = GeneratePlans(generator);
    const int son_plans      = GeneratePlans(generator);
    const int daughter_plans = GeneratePlans(generator);

    Account account(1'000'000);

    vector<future<int>> spend_futures;
    for (const int plans : {mother_plans, father_plans, son_plans, daughter_plans}) {
        spend_futures.push_back(async(SpendAll, ref(account), plans));
    }

    for (auto& spend_future : spend_futures) {
        cout << "Spent " << spend_future.get() << endl;
    }

    cout << account.GetValue() << endl;
}

```

Обратите внимание на метод  `TrySpendOne`. Метод  `fetch_sub`  атомарно вычитает единицу и возвращает значение, которое было в переменной непосредственно перед этой операцией. И если изначальное значение уже было неположительным, значит, всё потратили до нас и зря мы спешили вычесть единицу. В случае неуспеха всё возвращается на место — и какой бы процесс не пришёл между поспешным вычитанием и возвратом, ему всё равно не на что надеяться, потому что после возврата баланс не станет положительным.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Что можно сказать о работе такой программы? Выберите несколько вариантов ответа.

-   **(-)**  Перетрат не будет, и баланс не уйдёт в минус.

> К сожалению, нет: два процесса всё ещё могут одновременно выполнить сравнение, перейти в  `else`-ветку и там последовательно вычесть каждый свой  `plan`  из  `value_`, уведя его в минус.

-   **(+)**  Баланс может уйти в минус.
    
-   **(-)**  Как и раньше, сумма трат каждого человека может оказаться больше суммарного изменения переменной  `value_`.
    

> А вот здесь всё будет в порядке: вычитания  `plan`  из  `value_`  не будут конфликтовать друг с другом.

</details>

<details>
<summary>Задание:</summary>

## Задание

В прошлом уроке вы реализовали функцию  `CopyIfUnordered`  для копирования элементов с фильтрацией, эффективную благодаря неупорядоченности результата и применению многопоточности. Перепишите  `CopyIfUnordered`  на atomic-типы вместо мьютексов. Задача аналогична предыдущей, но в программе не должна встречаться подстрока  `mutex`.

### Подсказка

Используйте конструктор или метод  `resize`, чтобы выделить память для вектора с запасом. Храните настоящий размер в атомарной переменной  `size`. Тогда вставка нового элемента — это его запись в  `result[size]`  с последующим увеличением размера. Но учтите: не конфликтуют друг с другом только операции непосредственного использования или изменения атомарной переменной. Операция  `result[size] = value;`  может выполниться в двух потоках одновременно для одного и того же  `size`, и это приведёт к состоянию гонки.

</details>

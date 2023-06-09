<details>
<summary>Теория + тестирование:</summary>

# Сравнение, копирование и присваивание

В прошлом уроке вы реализовали поддержку STL-совместимых итераторов. Теперь можно обходить элементы списка и проводить над ними различные манипуляции. В частности, вам стал доступен широкий спектр стандартных алгоритмов, принимающих прямые и входные итераторы. Например,  `std::find`:

```cpp
int main() {
    using namespace std;
    SingleLinkedList<int> numbers;
    ...
    if (const auto it = std::find(numbers.cbegin(), numbers.cend(), 42); it != numbers.cend()) {
        cout << "The list contains 42"s << endl;
    } else {
        cout << "The list doesn't contain 42"s << endl;
    }
}

```

В этом уроке сделаем использование списка более удобным. Вы реализуете:

-   поддержку сравнения списков,
-   конструирование списка с указанием содержимого,
-   обмен содержимого списков,
-   копирование и присваивание.

### Сравнение списков

Содержимое стандартных контейнеров, таких как  `vector`, можно сравнивать, используя операции  `==`,  `!=`,  `>=`,  `<=`,  `>`,  `<`, когда эти операции определены для их элементов.

Два контейнера одного типа считаются равными, когда их размеры равны и в них содержатся равные элементы. Теперь, когда список поддерживает итераторы, реализовать операцию  `==`  можно, применив алгоритм  `std::equal`.

Операция  `<`  возвращает  `true`, когда левый список лексикографически предшествует правому списку. Её можно реализовать, используя алгоритм  [`std::lexicographical_compare`](https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare). Например, список  `{1, 2, 3}`  лексикографически предшествует спискам  `{1, 2, 4}`,  `{1, 3}`  и  `{1, 2, 3, 1}`.

Остальные операции можем реализовать на основе  `==`  и  `<`. Например, выражение  `list1 > list2`  эквивалентно  `list2 < list1`, а  `list1 <= list2`  — противоположно  `list2 < list1`.

### Обмен содержимого списков

У многих классов стандартной библиотеки есть метод  `swap`, обменивающий состояние объекта с другим объектом того же типа без выбрасывания исключений. Функция  `swap`  лежит в основе идиомы copy-and-swap и помогает писать код, устойчивый к исключениям. Когда разработаете метод  `swap`  в классе  `SingleLinkedList`, вам станет проще реализовать надёжное конструирование, копирование и присваивание односвязных списков.

Реализовать метод  `swap`  довольно просто. Элементы списка хранятся в динамической памяти, а сам список хранит лишь адрес своего первого элемента и размер. Чтобы обменять содержимое двух списков, надо обменять размеры списков и указатели на первые элементы.

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%A1%D1%80%D0%B0%D0%B2%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5_%D0%BA%D0%BE%D0%BF%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5_%D0%B8_%D0%BF%D1%80%D0%B8%D1%81%D0%B2%D0%B0%D0%B8%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5/img/1.png?raw=true)

_Процесс обмена содержимого списков_

Такой обмен выполняется за время O(1), не требует выделения динамической памяти и не выбрасывает исключений.

Для удобства реализуем шаблонную функцию  `swap`, использующую метод  `swap`. Она позволит без лишнего копирования и выбрасывания исключений обменивать содержимое двух списков, как это делает  `std::swap`. Для каждого из стандартных контейнеров есть специализированная версия функции  `swap`, основанная на вызове метода  `swap`  соответствующего контейнера:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {...}
    ...
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

int main() {
    SingleLinkedList<int> list_1;
    SingleLinkedList<int> list_2;
    ...
    swap(list_1, list_2);
}

```

----------

Если функцию  `swap`  декларировать в h-файле, нужно ли её реализацию выносить в cpp-файл?

-   Да, иначе программа не скомпонуется.
    
-   Нет, реализации должны быть там же, где и декларации.
    
-   Нет, так как функция шаблонная.
    

Декларация шаблонной функции должна быть доступна при её настройке, поэтому шаблонные функции нужно размещать в h-файле. Если вам заранее известен перечень всех возможных специализаций, тело функции можно разместить в cpp-файле, явно указывая, какие специализации потребуются.

### Создание списка из std::initializer_list

Стандартные контейнеры поддерживают свою инициализацию путём передачи набора элементов напрямую в конструктор:

```cpp
#include <cassert>
#include <vector>

using namespace std;

int main() {
    vector<int> numbers{10, 20, 30, 40, 50};
    assert(numbers.size() == 5u);
    assert(numbers[2] == 30);
}

```

Это не какая-нибудь магия, доступная только классам стандартной библиотеки. Компилятор, встречая список элементов внутри фигурных скобок, конструирует экземпляр класса  [`std::initializer_list`](https://en.cppreference.com/w/cpp/utility/initializer_list). Применив этот легковесный шаблонный класс, можно получить доступ к созданному списку элементов. Один из конструкторов класса  `vector`  принимает параметр типа  `initializer_list`  и конструирует вектор, содержащий копию элементов переданного списка. Подобный конструктор есть и у других классов стандартной библиотеки.

`initializer_list`  имеет методы  `size`,  `begin`  и  `end`. Они позволяют узнать его размер и перебрать элементы списка инициализации:

```cpp
#include <initializer_list>
#include <cassert>

int main() {
    std::initializer_list<int> items = {2, 3, 4};
    
    assert(items.size() == 3);
    
    int sum = 0;
    for (auto item : items) {
        sum += item;
    }
    assert(sum == 9);
    
    auto it = items.begin();
    int product = 1;
    while (it != items.end()) {
        product *= *it;
        ++it;
    }
    assert(product == 24);
}

```

Добавим классу  `SingleLinkedList`  конструктор, принимающий  `initializer_list`, и сможем совместить инициализацию списка с наполнением его элементами.

![2.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%A1%D1%80%D0%B0%D0%B2%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5_%D0%BA%D0%BE%D0%BF%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5_%D0%B8_%D0%BF%D1%80%D0%B8%D1%81%D0%B2%D0%B0%D0%B8%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5/img/2.png?raw=true)

_`initializer_list`  и список, который из него сконструирован_

```cpp
template <typename Type>
class SingleLinkedList {
public:
    SingleLinkedList(std::initializer_list<Type> values) {...}
    ...
};

int main() {
    SingleLinkedList<int> list{1, 2, 3, 4};
    assert(list.GetSize() == 4);
    assert(*list.begin() == 1);
}

```

`initializer_list`  принимают по значению, так как это легковесный объект. При его копировании не происходит глубокое копирование элементов.

### Копирование списка

Как вы наверняка догадались, сгенерированный компилятором конструктор копирования для класса  `SingleLinkedList`  не подойдёт. Копия будет использовать те же узлы, что и оригинальный список, и при разрушении одного из списков указатель на первый узел второго списка станет невалидным, что приведёт к неопределённому поведению. Точь-в-точь как у осьминогов со сросшимися щупальцами. А это значит, что для односвязного списка нужен пользовательский конструктор копирования, который создаст копию элементов исходного списка.

Конструктор копирования важно сделать безопасным к возникновению исключений — их выбрасывание во время работы не должно приводить к утечкам памяти и неопределённому поведению. Ещё следует помнить, что при выбрасывании исключения во время конструирования объекта деструктор создаваемого объекта вызван не будет.

Один из самых простых способов сделать надёжный конструктор копирования — применить идиому copy-and-swap:

-   Создать внутри конструктора копирования временный односвязный список и последовательно скопировать внутрь него элементы исходного списка. Если на этом этапе будет выброшено исключение, деструктор временного списка освободит память от его элементов.
-   Когда временный список будет содержать копию исходного списка, останется использовать метод  `swap`  и обменять состояние текущего экземпляра класса и временного списка.

```cpp
template <typename Type>
class SingleLinkedList {
public:
    SingleLinkedList(const SingleLinkedList& other) {
        // Сначала надо удостовериться, что текущий список пуст
        assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp;
        
        /* скопировать внутрь tmp элементы other */

        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }
    ...
};

```

Чтобы написать надёжный код на C++, используйте идиому RAII. С ней код будет ещё и более простым, так как своевременный вызов деструктора обеспечивается компилятором.

Вы встречались с этой идиомой ранее, когда писали профилировщик. Идиома RAII заключается в том, что время жизни ресурса неразрывно связывается со временем жизни объекта — в конструкторе происходит получение ресурса, а в деструкторе — освобождение.

Здесь владение элементами списка временно поручается локальной переменной  `tmp`. Так копируемые элементы списка будут находиться под управлением этой переменной, пока не совершится операция обмена.

Вы ещё вернётесь к подробному изучению идиомы RAII в будущих спринтах, а пока рекомендуем ознакомиться с ней здесь:  [https://en.cppreference.com/w/cpp/language/raii](https://en.cppreference.com/w/cpp/language/raii)

При конструировании односвязного списка на основе  `initializer_list`  также пригодится идиома copy-and-swap.

### Присваивание списка

Операцию присваивания, последнюю из «Правила трёх»‎, можно легко и надёжно реализовать, применив идиому copy-and-swap:

-   Проверьте, не выполняется присваивание списка самому себе, сравнив адреса левого и правого аргументов операции присваивания.
-   Сконструируйте временную копию правого аргумента. Выбрасывание исключения на этом этапе никак не повлияет на состояние текущего объекта (левый аргумент операции присваивания).
-   Используйте метод  `swap`, чтобы обменять содержимое временной копии и текущего объекта. Операция  `swap`  исключений не выбрасывает и выполняется быстро. Временная копия будет содержать предыдущее значение левого аргумента, а текущий экземпляр — копию правого аргумента операции присваивания.
-   При выходе из операции  `=`  временный объект будет разрушен, освободив память от предыдущего содержимого списка.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания


Если функцию  `swap`  декларировать в h-файле, нужно ли её реализацию выносить в cpp-файл?

-   **(-)**  Да, иначе программа не скомпонуется.
    
-   **(-)**  Нет, реализации должны быть там же, где и декларации.
    
-   **(+)**  Нет, так как функция шаблонная.

</details>

<details>
<summary>Задание:</summary>

## Задание

Реализуйте в классе односвязного списка следующий функционал:

-   Операции сравнения  `==`,  `!=`,  `<`,  `>`,  `<=`,  `>=`;
-   Обмен содержимого двух списков с использованием метода  `swap`  и шаблонной функции  `swap`;
-   Конструирование односвязного списка на основе  `initializer_list`. Последовательность элементов созданного списка и  `initializer_list`  должна быть одинаковой;
-   Надёжные конструктор копирования и операцию присваивания. Операция присваивания должна обеспечивать строгую гарантию безопасности исключений. Если в процессе присваивания будет выброшено исключение, содержимое левого аргумента операции присваивания должно остаться без изменений.

Сигнатура добавляемых операций:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    SingleLinkedList(std::initializer_list<Type> values) {
        // Реализуйте конструктор самостоятельно
    }

    SingleLinkedList(const SingleLinkedList& other) {
        // Реализуйте конструктор самостоятельно
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        // Реализуйте присваивание самостоятельно
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        // Реализуйте обмен содержимого списков самостоятельно
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    // Реализуйте обмен самостоятельно
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return true;
}

```

Пример использования и тесты:

```cpp
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {...};

// Эта функция проверяет работу класса SingleLinkedList
void Test3() {
    // Проверка списков на равенство и неравенство
    {
        SingleLinkedList<int> list_1;
        list_1.PushFront(1);
        list_1.PushFront(2);

        SingleLinkedList<int> list_2;
        list_2.PushFront(1);
        list_2.PushFront(2);
        list_2.PushFront(3);

        SingleLinkedList<int> list_1_copy;
        list_1_copy.PushFront(1);
        list_1_copy.PushFront(2);

        SingleLinkedList<int> empty_list;
        SingleLinkedList<int> another_empty_list;

        // Список равен самому себе
        assert(list_1 == list_1);
        assert(empty_list == empty_list);

        // Списки с одинаковым содержимым равны, а с разным - не равны
        assert(list_1 == list_1_copy);
        assert(list_1 != list_2);
        assert(list_2 != list_1);
        assert(empty_list == another_empty_list);
    }

    // Обмен содержимого списков
    {
        SingleLinkedList<int> first;
        first.PushFront(1);
        first.PushFront(2);

        SingleLinkedList<int> second;
        second.PushFront(10);
        second.PushFront(11);
        second.PushFront(15);

        const auto old_first_begin = first.begin();
        const auto old_second_begin = second.begin();
        const auto old_first_size = first.GetSize();
        const auto old_second_size = second.GetSize();

        first.swap(second);

        assert(second.begin() == old_first_begin);
        assert(first.begin() == old_second_begin);
        assert(second.GetSize() == old_first_size);
        assert(first.GetSize() == old_second_size);

        // Обмен при помощи функции swap
        {
            using std::swap;

            // В отсутствие пользовательской перегрузки будет вызвана функция std::swap, которая
            // выполнит обмен через создание временной копии
            swap(first, second);

            // Убеждаемся, что используется не std::swap, а пользовательская перегрузка

            // Если бы обмен был выполнен с созданием временной копии,
            // то итератор first.begin() не будет равен ранее сохранённому значению,
            // так как копия будет хранить свои узлы по иным адресам
            assert(first.begin() == old_first_begin);
            assert(second.begin() == old_second_begin);
            assert(first.GetSize() == old_first_size);
            assert(second.GetSize() == old_second_size);
        }
    }

    // Инициализация списка при помощи std::initializer_list
    {
        SingleLinkedList<int> list{1, 2, 3, 4, 5};
        assert(list.GetSize() == 5);
        assert(!list.IsEmpty());
        assert(std::equal(list.begin(), list.end(), std::begin({1, 2, 3, 4, 5})));
    }

    // Лексикографическое сравнение списков
    {
        using IntList = SingleLinkedList<int>;

        assert((IntList{1, 2, 3} < IntList{1, 2, 3, 1}));
        assert((IntList{1, 2, 3} <= IntList{1, 2, 3}));
        assert((IntList{1, 2, 4} > IntList{1, 2, 3}));
        assert((IntList{1, 2, 3} >= IntList{1, 2, 3}));
    }

    // Копирование списков
    {
        const SingleLinkedList<int> empty_list{};
        // Копирование пустого списка
        {
            auto list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }

        SingleLinkedList<int> non_empty_list{1, 2, 3, 4};
        // Копирование непустого списка
        {
            auto list_copy(non_empty_list);

            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }

    // Присваивание списков
    {
        const SingleLinkedList<int> source_list{1, 2, 3, 4};

        SingleLinkedList<int> receiver{5, 4, 3, 2, 1};
        receiver = source_list;
        assert(receiver.begin() != source_list.begin());
        assert(receiver == source_list);
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    // Безопасное присваивание списков
    {
        SingleLinkedList<ThrowOnCopy> src_list;
        src_list.PushFront(ThrowOnCopy{});
        src_list.PushFront(ThrowOnCopy{});
        auto thrower = src_list.begin();
        src_list.PushFront(ThrowOnCopy{});

        int copy_counter = 0;  // при первом же копировании будет выброшего исключение
        thrower->countdown_ptr = &copy_counter;

        SingleLinkedList<ThrowOnCopy> dst_list;
        dst_list.PushFront(ThrowOnCopy{});
        int dst_counter = 10;
        dst_list.begin()->countdown_ptr = &dst_counter;
        dst_list.PushFront(ThrowOnCopy{});

        try {
            dst_list = src_list;
            // Ожидается исключение при присваивании
            assert(false);
        } catch (const std::bad_alloc&) {
            // Проверяем, что состояние списка-приёмника не изменилось
            // при выбрасывании исключений
            assert(dst_list.GetSize() == 2);
            auto it = dst_list.begin();
            assert(it != dst_list.end());
            assert(it->countdown_ptr == nullptr);
            ++it;
            assert(it != dst_list.end());
            assert(it->countdown_ptr == &dst_counter);
            assert(dst_counter == 10);
        } catch (...) {
            // Других типов исключений не ожидается
            assert(false);
        }
    }
}

int main() {
    Test3();
}

```

### Ограничения

Инкремент и разыменование итератора, ссылающегося на позицию за последним элементом списка, — недопустимые операции. Очистка или разрушение списка делают невалидными его итераторы. Любые операции над невалидным итератором, кроме естественного вызова деструктора, тоже недопустимы. Гарантируется, что тренажёр не будет совершать таких операций. Не усложняйте класс списка и итератора, чтобы выявить ситуации некорректного использования итераторов.

### Что отправлять на проверку

В решении должен быть шаблонный класс  `SingleLinkedList`  с перегруженными операциями, функцией  `swap`  и нужными для их работы директивами  `#include`. Функция  `main`  игнорируется.

### Как будет тестироваться ваш код

Сохраните сигнатуры всех публичных методов класса  `SingleLinkedList`, его итераторов и операций неизменными, чтобы код скомпилировался без ошибок.

### Подсказка

У конструктора копирования и конструктора из  `std::initializer_list`  есть нечто общее — и тот, и другой принимают на вход объект, у которого есть итераторы  `begin`  и  `end`. Эти итераторы задают диапазон элементов, которые надо скопировать. Если вынести инициализацию списка элементами из переданного диапазона итераторов в отдельный шаблонный метод, избавитесь от дублирования кода.

</details>

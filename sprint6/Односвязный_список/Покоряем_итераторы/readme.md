<details>
<summary>Теория + тестирование:</summary>

# Покоряем итераторы

Ваш односвязный список содержит методы  `IsEmpty`  и  `GetSize`, которые позволяют узнать о наличии и количестве элементов, а также методы  `PushFront`  и  `Clear`  для вставки элемента и очистки списка. Но полноценная работа со списками предполагает не только возможность помещать внутрь него элементы, но и проходить по элементам списка.

Стандартные контейнеры вводят концепцию итератора — специального объекта, который играет роль указателя на элемент контейнера. Используем пару итераторов, чтобы задать подлежащий обходу диапазон  `[начало, конец)`  элементов контейнера.

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%9F%D0%BE%D0%BA%D0%BE%D1%80%D1%8F%D0%B5%D0%BC_%D0%B8%D1%82%D0%B5%D1%80%D0%B0%D1%82%D0%BE%D1%80%D1%8B/img/1.png?raw=true)

_Итераторы  `begin`,  `before_begin`  и  `end`  и узлы, на которые они указывают_

Итераторы и полуинтервалы — абстракции, которые прячут особенности внутреннего устройства конкретного контейнера за унифицированным интерфейсом. Итераторы разных контейнеров похожи: они предоставляют операции  `*`  и  `->`  для доступа к элементам, операции  `++`  и  `--`  для обхода элементов в прямом и обратном направлении. Благодаря этой унификации и шаблонам один алгоритм способен обрабатывать содержимое различных контейнеров. Создавать под каждый отдельную версию алгоритма не нужно.

В этом уроке вы реализуете поддержку обхода элементов для класса  `SingleLinkedList`, совместимую со стандартной библиотекой и циклом range-based for.

### Как устроен итератор односвязного списка

Чтобы класс или структура с точки зрения стандартных алгоритмов считались итератором, внутри должны быть объявлены вложенные типы:

-   `iterator_category`. Задаёт категорию итератора. Категория итератора сообщает стандартной библиотеке о наборе операций, поддерживаемых итератором. Так как односвязный список поддерживает перебор элементов только в прямом направлении, его категория итератора должна быть равна  `std::forward_iterator_tag`.
-   `value_type`. Задаёт тип элементов, доступ к которым предоставляет итератор. В случае односвязного списка этот тип совпадает с параметром  `Type`  шаблона  `SingleLinkedList`.
-   `difference_type`. Задаёт тип для хранения смещения между двумя итераторами. Используется в таких функциях, как  `std::advance`  и  `std::distance`. Для  `SingleLinkedList`  можно использовать тип  `std::ptrdiff_t`. Это целое число со знаком, разрядность которого совпадает с разрядностью указателя на целевой платформе.
-   `pointer`. Задаёт тип указателя, возвращаемого операцией  `->`. Для неконстантного итератора  `SingleLinkedList`  это  `Type*`, а для константного —  `const Type*`.
-   `reference`. Задаёт тип ссылки, которую возвращает операция разыменования  `*`. В зависимости от константности итератора  `SingleLinkedList`  это либо  `Type&`, либо  `const Type&`.

```cpp
#include <cstddef>
#include <iterator> // Содержит объявления категорий итераторов

template <typename Type>
class SingleLinkedList {
    // Шаблон класса «Базовый Итератор»
    // определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
        ...
    };
    ...
};

```

Константный итератор подобен указателю на константу. Он предоставляет доступ к элементам контейнера только для чтения, при этом значение самого итератора можно менять. Стандартные контейнеры имеют константную и неконстантную версии методов  `begin`  и  `end`, которые возвращают константный и неконстантный итераторы соответственно.

Итератор категории  `std::forward_iterator_tag`  соответствует категории однонаправленного итератора. В нём должны быть объявлены следующие операции:

-   Операции  `==`  и  `!=`. Итераторы одного и того же списка равны, если указывают на одну и ту же позицию. Константные и неконстантные итераторы одного и того же списка можно сравнивать друг с другом.
-   Операция инкремента  `++`. Перемещает итератор на следующую позицию в списке.
-   Операции  `*`  и  `->`  для доступа к элементу списка. Возвращают ссылку и указатель на значение, хранящееся в списке, а не на весь узел списка, задаваемый вложенной структурой  `Node`. Помните, итераторы должны скрывать внутреннее устройство контейнера от внешнего мира.

```cpp
template <typename Type>
class SingleLinkedList {
    template <typename ValueType>
    class BasicIterator {
    public:
        ...
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {...}
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {...}
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {...}
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {...}

        BasicIterator& operator++() noexcept {...}
        BasicIterator operator++(int) noexcept {...}

        [[nodiscard]] reference operator*() const noexcept {...}
        [[nodiscard]] pointer operator->() const noexcept {...}
    ...
    };
    ...
};

```

Чтобы использовать  `==`  и  `!=`  для сравнения константных и неконстантных итераторов, нужно объявить две версии этих операций.

Обратите внимание на перегрузку операции инкремента. Вспомним, что в C++ есть префиксная и постфиксная формы операции инкремента и декремента.

-   Префиксная версия модифицирует объект и возвращает ссылку на текущий экземпляр класса.
-   Постфиксная версия модифицирует объект и возвращает копию прежнего значения объекта.

Постфиксная форма операций инкремента и декремента принимает фиктивный параметр типа  `int`, который не используется и только подсказывает компилятору, какая версия переопределена. Как правило, постфиксная форма основывается на функционале префиксной:

```cpp
class MyType {
public:
    // Перегрузка префиксной формы
    MyType& operator++() {
        /* изменяем текущий объект */
        return *this;
    }
    // Перегрузка постфиксной формы
    MyType operator++(int) {
        auto old_value(*this); // Сохраняем прежнее значение объекта для последующего возврата
        ++(*this); // используем логику префиксной формы инкремента
        return old_value;
    }
    ...
};

```

Во внутреннем представлении итератор односвязного списка хранит указатель на узел списка. В операциях  `*`  и  `->`  этот указатель разыменовывается, а после инкремента итератора ссылается на следующий узел списка.

Чтобы защитить внутреннюю структуру списка от доступа извне, создавать итератор, ссылающийся на позицию внутри списка, можно только внутри  `SingleLinkedList`. Для этого в конструктор итератора передаётся указатель на приватную структуру  `Node`.

Приватные члены класса доступны изнутри него. Так, например,  `BasicIterator`  находится внутри класса  `SingleLinkedList`, поэтому итератору доступны все приватные члены  `SingleLinkedList`, в том числе  `Node`. Но наоборот это не работает:  `SingleLinkedList`  находится не внутри, а снаружи  `BasicIterator`. Списку доступ к приватным членам итератора закрыт.

Но некоторым операциям списка может понадобиться доступ к указателю на узел внутри итератора. В C++ класс или структура могут объявить о своём безграничном доверии другому классу, структуре или функции, назвав их дружественными. Для этого применяют ключевое слово  `friend`. Класс открывает друзьям доступ к своей приватной области.

Пользуйтесь этой возможностью осторожно: неаккуратная работа с приватными полями класса способна нарушить целостность состояния объекта. Например, после изменения приватной части класса программист может забыть внести нужные правки в логику работы его друзей. Вероятность этой ситуации повышается, если класс и его друзья находятся в разных файлах. В лучшем случае код не скомпилируется, а в худшем — скомпилируется, но ошибка долго останется незамеченной.

По умолчанию класс  `SingleLinkedList`  не имеет доступа к приватной области своего итератора. Объявив список своим другом, итератор разрешит ему, и только ему обращаться к своей внутренней части. В данном случае такое доверие обоснованно, так как и список, и итератор работают с одной и той же структурой данных, скрытой от внешнего мира:

```cpp
template <typename Type>
class SingleLinkedList {
    template <typename ValueType>
    class BasicIterator {
       ...
    private:
        // Разрешаем SingleLinkedList обращаться к приватной области
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {...}
        Node* node_ = nullptr;
    };

public:
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        // Благодаря дружбе SingleLinkedList имеет доступ к приватному конструктору своего итератора
        return Iterator{head_.next_node};
    }
    ...
};

```

Дружба — это чудо!＊

＊Отношение дружбы — самая сильная связь в C++. Гибкие и надёжные архитектуры строятся из компонентов, минимально осведомлённых об устройстве друг друга. Классу редко следует открывать внешнему коду доступ к своей приватной области. Контейнер и его итератор — один из случаев, когда это может быть оправдано.

----------

В примере выше будет ли  `SingleLinkedList`  иметь доступ к приватной части итератора, объявленного в списке другого типа? Например, будет ли  `SingleLinkedList<string>`  иметь доступ к  `SingleLinkedList<int>::BasicIterator<int>`?

Для ответа на этот вопрос воспользуйтесь компилятором.

-   Нет, не будет — имя  `SingleLinkedList`  в конструкции  `friend`  означает список, настроенный на конкретный тип.
    
-   Да, будет — имя  `SingleLinkedList`  в конструкции  `friend`  означает шаблонный класс и применяется к любой его специализации.
    
-   Нет, не будет. Конструкция  `friend`  даёт доступ не из  `SingleLinkedList`  к  `BasicIterator`, а наоборот.
    

Компилятор интерпретирует конструкцию  `friend`  во время настройки класса  `SingleLinkedList`  на конкретный тип. Имя  `SingleLinkedList`  в этот момент — сокращение для  `SingleLinkedList<Type>`, и дружба будет применяться только к этой специализации. Если вы хотите подружить класс со всеми возможными специализациями шаблонного класса, то саму конструкцию  `friend`  можно сделать шаблонной:

```cpp
#include <utility>

class Alibaba;

// Разбойник — это объект шаблонного класса
// Разбойники разных специализаций имеют разные шаблонные параметры
template <typename LootType>
class Thief {
public:
    void GreetAlibaba(Alibaba& alibaba);

    LootType ConvertMoneyToLoot(int money);

private:
    LootType loot_;
};

class Alibaba {
    // Али-Баба дружит с разбойниками всех специализаций
    // Конструкцию friend делаем шаблонной
    template <typename T>
    friend class Thief;

private:
    int money_ = 100;
};

template <typename T>
void Thief<T>::GreetAlibaba(Alibaba& alibaba) {
    int stolen = 0;
    // Благодаря дружбе с Али-Бабой разбойник получил доступ к 
    // его приватному полю и обнулил это поле
    std::swap(stolen, alibaba.money_);
    loot_ += ConvertMoneyToLoot(stolen);
}

```

### Поддержка обхода элементов

Как и многое в C++, проход по элементам циклом range-based for — не привилегия, доступная лишь стандартным контейнерам. Поддержку этого цикла можно добавить любому классу, включив в него публичные методы  `begin`  и  `end`. Они возвращают STL-совместимые итераторы, задающие диапазон элементов контейнера:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    [[nodiscard]] Iterator begin() noexcept {...}
    [[nodiscard]] Iterator end() noexcept {...}
    // Константные версии begin/end для обхода списка без возможности модификации его элементов
    [[nodiscard]] ConstIterator begin() const noexcept {...}
    [[nodiscard]] ConstIterator end() const noexcept {...}
    // Методы для удобного получения константных итераторов у неконстантного контейнера
    [[nodiscard]] ConstIterator cbegin() const noexcept {...}
    [[nodiscard]] ConstIterator cend() const noexcept {...}
    ...
};

int main() {
    SingleLinkedList<int> list;
    ...
    // Благодаря методам begin и end SingleLinkedList поддерживает range-based for
    for (int& item : list) {
        *item *= 2;
    }
}
```

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

----------

В примере выше будет ли  `SingleLinkedList`  иметь доступ к приватной части итератора, объявленного в списке другого типа? Например, будет ли  `SingleLinkedList<string>`  иметь доступ к  `SingleLinkedList<int>::BasicIterator<int>`?

Для ответа на этот вопрос воспользуйтесь компилятором.

-   **(+)**  Нет, не будет — имя  `SingleLinkedList`  в конструкции  `friend`  означает список, настроенный на конкретный тип.
    
-   **(-)**  Да, будет — имя  `SingleLinkedList`  в конструкции  `friend`  означает шаблонный класс и применяется к любой его специализации.
    
-   **(-)**  Нет, не будет. Конструкция  `friend`  даёт доступ не из  `SingleLinkedList`  к  `BasicIterator`, а наоборот.

</details>

<details>
<summary>Задание:</summary>

## Задание

Разработайте поддержку перебора элементов контейнера  `SingleLinkedList`.

-   Реализуйте шаблонный класс  `BasicIterator`, на основе которого будут объявлены константный и неконстантный итераторы списка.
-   В классе списка реализуйте константную и неконстантную версии методов  `begin`  и  `end`, которые возвращают итераторы на первый элемент контейнера и позицию, следующую за последним элементом.
-   Чтобы получать константные итераторы было удобно, реализуйте методы  `cbegin`  и  `cend`.

Сигнатура добавляемых методов:

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            assert(false);
            // Реализуйте конструктор самостоятельно
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            assert(false);
            // Реализуйте конструктор самостоятельно
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            assert(false);
            // Не реализовано
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            assert(false);
            // Заглушка. Реализуйте оператор самостоятельно
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        assert(false);
        // Реализуйте самостоятельно
        return {};
    }

    ...
};

```

### Пример использования и тесты

```cpp
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {...};

// Эта функция тестирует работу SingleLinkedList
void Test2() {
    // Итерирование по пустому списку
    {
        SingleLinkedList<int> list;
        // Константная ссылка для доступа к константным версиям begin()/end()
        const auto& const_list = list;

        // Итераторы begin и end у пустого диапазона равны друг другу
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // Итерирование по непустому списку
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);

        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // Проверка прединкремента
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Проверка постинкремента
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // Итератор, указывающий на позицию после последнего элемента, равен итератору end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // Преобразование итераторов
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // Конструирование ConstIterator из Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // Присваивание ConstIterator'у значения Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // Проверка оператора ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

int main() {
    Test2();
}

```

### Ограничения

Инкремент и разыменование итератора, ссылающегося на позицию за последним элементом списка, — недопустимые операции. Очистка или разрушение списка делают невалидными его итераторы. Любые операции над невалидным итератором, кроме естественного вызова деструктора, тоже недопустимы. Гарантируется, что тренажёр не будет совершать таких операций. Не усложняйте класс списка и итератора, чтобы выявить ситуации некорректного использования итераторов.

### Требование

Сохраните сигнатуры всех публичных методов класса  `SingleLinkedList`  и его итераторов неизменными, чтобы код скомпилировался без ошибок.

### Что отправлять на проверку

В решении должен быть класс  `SingleLinkedList`  и нужные для его работы директивы  `#include`. Функция  `main`  учитываться не будет.

### Как будет тестироваться ваш код

Будет проверена корректная работа итераторов и методов, возвращающих итераторы по аналогии с тестами в условии.

### Подсказка

Не переживайте, что  `end`-итератор, который указывает на позицию, следующую за последним элементом, будет ссылаться на  `nullptr`. Это корректная операция, ведь разыменование и инкремент итератора не разрешены.

</details>

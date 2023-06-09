<details>
<summary>Теория + тестирование:</summary>

# Вставка и удаление в произвольной позиции

В прошлом уроке вы реализовали операции над списком в целом: сравнение, присваивание, обмен содержимым. На практике часто возникает потребность в ‎точечной‎ модификации списка: вставка и удаление элементов из произвольной позиции списка. В этом уроке вы разработаете эти операции.

### Вставка элемента в произвольную позицию списка

Вспомним структуру односвязного списка. Его элементы хранятся в узлах, последовательно связанных друг с другом. Эти узлы представлены структурой  `Node`, которая хранит значение элемента и указатель на следующий узел.

----------

Адрес какого узла нужно знать, чтобы вставить новый элемент в произвольное место односвязного списка?

-   Узла, который предшествует вставляемому элементу
    
-   Узла, который следует за вставляемым элементом
    

Итак, чтобы вставить новый узел, нужно иметь доступ к узлу, который предшествует позиции вставки. Проблема возникнет при вставке в начало: полноценного предшествующего узла в этом случае не будет. Тут пригодится фиктивный узел  `head_`, который расположен «‎перед первым элементом»‎. Чтобы получить итератор на него, используем методы  `before_begin`  и  `cbefore_begin`:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    [[nodiscard]] Iterator before_begin() noexcept;
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept;
    [[nodiscard]] ConstIterator before_begin() const noexcept;
    ...
};
```

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%92%D1%81%D1%82%D0%B0%D0%B2%D0%BA%D0%B0_%D0%B8_%D1%83%D0%B4%D0%B0%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%B2_%D0%BF%D1%80%D0%BE%D0%B8%D0%B7%D0%B2%D0%BE%D0%BB%D1%8C%D0%BD%D0%BE%D0%B9_%D0%BF%D0%BE%D0%B7%D0%B8%D1%86%D0%B8%D0%B8/img/1.png?raw=true)

_Список из трёх элементов и итераторы  `begin`,  `before_begin`,  `end`_

Вот алгоритм вставки значения  `X`  после узла  `N`:

1.  Создаём новый узел со значением  `X`.
2.  С нового узла ссылаемся на  `N.next_node`.
3.  С  `N`  ссылаемся на новый узел.

![2.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%92%D1%81%D1%82%D0%B0%D0%B2%D0%BA%D0%B0_%D0%B8_%D1%83%D0%B4%D0%B0%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%B2_%D0%BF%D1%80%D0%BE%D0%B8%D0%B7%D0%B2%D0%BE%D0%BB%D1%8C%D0%BD%D0%BE%D0%B9_%D0%BF%D0%BE%D0%B7%D0%B8%D1%86%D0%B8%D0%B8/img/2.png?raw=true)

_Вставка числа 50 после элемента 20 в список с элементами 10, 20, 30, 40. pos показывает позицию вставки, а p — возвращаемый итератор_

После вставки нужно увеличить размер списка и вернуть итератор, который ссылается на вновь созданный узел. Возвращённый итератор можем использовать, например, для вставки следующего элемента:

```cpp
int main() {
    SingleLinkedList<int> list;
    auto pos = list.InsertAfter(list.cbefore_begin(), 1);
    pos = list.InsertAfter(pos, 2);
    list.InsertAfter(pos, 3);
    // Список содержит {1, 2, 3}
}

```

При вставке нужно выделить динамическую память и скопировать вставляемое значение. Это может привести к выбрасыванию исключения. Чтобы метод  `InsertAfter`  обеспечивал строгую гарантию безопасности исключений, примените подход, использованный в методе  `PushFront`, — сначала создайте узел в динамической памяти, а потом обновите связи между узлами и размер списка.

### Удаление элемента из начала списка

Обозначим первый узел как  `N`  и опишем процесс его удаления:

-   указатель фиктивного элемента начинает указывать на  `N.next_node`,
-   узел  `N`  удаляется.

Эта операция выполняется за время O(1). Она не требует динамического выделения памяти, поэтому не выбрасывает исключений. Если, конечно, их не выбрасывает деструктор типа  `Type`.

![3.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%92%D1%81%D1%82%D0%B0%D0%B2%D0%BA%D0%B0_%D0%B8_%D1%83%D0%B4%D0%B0%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%B2_%D0%BF%D1%80%D0%BE%D0%B8%D0%B7%D0%B2%D0%BE%D0%BB%D1%8C%D0%BD%D0%BE%D0%B9_%D0%BF%D0%BE%D0%B7%D0%B8%D1%86%D0%B8%D0%B8/img/3.png?raw=true)

_Процесс удаления первого узла из списка с элементами 10, 20, 30, 40_

К пустому списку операцию удаления первого элемента применить нельзя.

### Удаление элемента в произвольной позиции

----------

Адрес какого узла нужно знать, чтобы удалить произвольный элемент односвязного списка? Выберите один ответ.

-   Удаляемого узла
    
-   Узла, который предшествует удаляемому элементу
    
-   Следующего узла
    

Чтобы удалить элемент в произвольной позиции односвязного списка, нужен доступ к узлу, который предшествует удаляемому элементу. После удаления указатель с предшествующего узла будет ссылаться на узел, следующий за удаляемым. Эта операция выполняется за время O(1), так как не требует доступа к другим узлам односвязного списка.

Такая операция не выбрасывает исключений, потому что не требует динамического выделения памяти. Операция  `delete`, вызываемая, чтобы удалить узел списка, тоже не выбрасывает исключений. Исключения могут возникнуть, если их выбросит деструктор типа  `Type`, но это нетипичная ситуация — лучше никогда не выбрасывать исключений в деструкторах.

![4.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9E%D0%B4%D0%BD%D0%BE%D1%81%D0%B2%D1%8F%D0%B7%D0%BD%D1%8B%D0%B9_%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA/%D0%92%D1%81%D1%82%D0%B0%D0%B2%D0%BA%D0%B0_%D0%B8_%D1%83%D0%B4%D0%B0%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%B2_%D0%BF%D1%80%D0%BE%D0%B8%D0%B7%D0%B2%D0%BE%D0%BB%D1%8C%D0%BD%D0%BE%D0%B9_%D0%BF%D0%BE%D0%B7%D0%B8%D1%86%D0%B8%D0%B8/img/4.png?raw=true)

_Удаление элемента 30 из списка с элементами 10, 20, 30, 40. Итератор  `pos`  обозначает удаляемый узел, а  `p`  — узел, следующий за удалённым_

Метод  `EraseAfter`  возвращает итератор, который ссылается на элемент, следующий за удалённым, либо на  `end`, если произошло удаление последнего элемента списка. Возвращённый итератор можно использовать для обработки следующих элементов.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Адрес какого узла нужно знать, чтобы вставить новый элемент в произвольное место односвязного списка?

-   **(+)**  Узла, который предшествует вставляемому элементу

> В односвязном списке каждый узел ссылается на следующий за ним элемент. Предыдущий узел должен сослаться на вставляемый, а вставляемый — на следующий.

-   **(-)**  Узла, который следует за вставляемым элементом

> В односвязном списке каждый узел ссылается на следующий за ним элемент. Зная лишь адрес следующего узла, невозможно получить доступ к предыдущему. А предыдущий должен сослаться на вставляемый узел.

----------

Адрес какого узла нужно знать, чтобы удалить произвольный элемент односвязного списка? Выберите один ответ.

-   **(-)**  Удаляемого узла
    
-   **(+)**  Узла, который предшествует удаляемому элементу
    
-   **(-)**  Следующего узла

</details>

<details>
<summary>Задание:</summary>

## Задание

Это задание — итоговый проект шестого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. А пока сохраните решение в своей IDE.

Реализуйте в классе односвязного списка следующие операции:

-   Метод  `PopFront`. Удаляет первый элемента непустого списка за время O(1). Не выбрасывает исключений.
-   Метод  `InsertAfter`. За время O(1) вставляет в список новое значение следом за элементом, на который ссылается переданный в  `InsertAfter`  итератор. Метод должен обеспечивать строгую гарантию безопасности исключений.
-   Метод  `EraseAfter`. За время O(1) удаляет из списка элемент, следующий за элементом, на который ссылается переданный в  `InsertAfter`  итератор. Не выбрасывает исключений.
-   Методы  `before_begin`  и  `cbefore_begin`. Возвращают итераторы, ссылающиеся на фиктивную позицию перед первым элементом списка. Такой итератор используется как параметр для методов  `InsertAfter`  и  `EraseAfter`, когда нужно вставить или удалить элемент в начале списка. Разыменовывать этот итератор нельзя.

Сигнатуры новых методов:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        // Реализуйте самостоятельно
        return {};
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        return {};
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

    void PopFront() noexcept {
        // Реализуйте метод самостоятельно
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }
    ...
};

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
void Test4() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{3, 14, 15, 92, 6};
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{1, 2, 3, 4};
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{1, 2, 3};
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
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

    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{}};
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{DeletionSpy{}, DeletionSpy{}, DeletionSpy{}};
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
}

int main() {
    Test4();
}

```

### Прекод

В качестве заготовки кода используйте своё решение из предыдущего урока.

### Ограничения

Не меняйте сигнатуры всех публичных методов класса  `SingleLinkedList`, его итераторов и операций, чтобы код скомпилировался без ошибок.

Считайте, что тип  `Type`  удовлетворяет следующим ограничениям:

-   Имеет конструктор без параметров.
-   Имеет копирующий конструктор.
-   Деструктор не выбрасывает исключений.

### Что отправлять на проверку

В решении должна быть реализация класса  `SingleLinkedList`  с перегруженными операциями, функцией  `swap`  и нужными для их работы директивами  `#include`. Функция  `main`  учитываться при проверке не будет.

### Как будет тестироваться ваш код

При тестировании не будут происходить следующие операции:

-   Удаление элемента в начале пустого списка.
-   Передача в методы  `InsertAfter`  и  `EraseAfter`  end-итератора текущего списка, невалидного итератора либо итератора, ссылающегося на элемент другого списка.

Не усложняйте класс списка и итератора, чтобы выявить ситуации некорректного использования итераторов.

### Подсказка

Пример использования и тесты из условия — лучшая подсказка.

</details>

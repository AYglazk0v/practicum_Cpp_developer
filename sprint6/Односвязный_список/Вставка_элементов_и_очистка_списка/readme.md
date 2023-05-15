<details>
<summary>Теория + тестирование:</summary>

# Вставка элементов и очистка списка

В предыдущем уроке вы познакомились с новой структурой данных — односвязным списком и создали каркас шаблонного класса  `SingleLinkedList`, который реализует функциональность пустого списка. В этом уроке вы научитесь динамически добавлять элементы в начало и очищать список.

### Вставка элемента в начало списка

Пусть дан список, где уже есть несколько элементов  `{5, 42, 8}`.

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Односвязный список с элементами  `{5, 42, 8}`_

Поле  `next_node`  фиктивного узла  `head_`  хранит указатель на первый элемент списка. В случае пустого списка  `head_.next_node`  равен  `nullptr`:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_В пустом списке указатель  `head_.next_node`  равен  `nullptr`_

Чтобы вставить новый элемент в начало списка, нужно создать новый узел. Следующим элементом этого узла будет текущий первый элемент списка, то есть значение  `head_.next_node`:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Чтобы вставить значение 17 в начало списка  `{5, 42, 8}`, создадим новый узел, который ссылается на первый узел списка_

Этот узел должен стать новым первым узлом списка. Обновим указатель  `head_.next_node`  и увеличим размер списка на единицу:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Вставленный узел становится первым узлом списка. Размер списка увеличивается_

----------

Выберите наиболее удачный вариант реализации  `PushFront`  для шаблона  `SingleLinkedList`. Учтите, что элемент типа  `Type`  может выбрасывать исключение в самых неожиданных случаях. Например, при присваивании.

-   **Вариант 1**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 2**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        front_node->value = value;
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    
private:
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 3**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (...) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 4**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (const std::bad_alloc&) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 5**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (...) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 6**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        Node* front_node = new Node();
        front_node->value = value;
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
    }
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

```

-   **Вариант 7**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        head_.next_node = new Node(value, head_.next_node);
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

### Очистка односвязного списка

Рассмотрим алгоритм очистки на примере односвязного списка  `{5, 42, 8`.

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Односвязный список с элементами  `{5, 42, 8}`_

Процесс очистки продолжается, пока список непустой — то есть указатель  `head_.next_node`  на первый элемент списка ненулевой. После первой итерации  `head_.next_node`  будет ссылаться на второй элемент со значением 42 по адресу  `head_.next_node->next_node`, а первый элемент со значением 5 удаляется:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Голова списка ссылается на его второй элемент, а первый элемент удаляется_

На следующих итерациях происходит то же самое. Указатель  `head_.next_node`  смещается на второй элемент списка  `{42, 8}`, а первый элемент удаляется:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Голова списка ссылается на значение 8, а значение 42 удаляется_

Когда в списке останется один элемент, его удаление приведёт к тому, что  `head_.next_node`  будет ссылаться на  `nullptr`:

____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________

_Удаляется последний элемент списка. Голова указывает на значение_

На этом процесс очистки списка заканчивается. Остаётся обнулить размер списка. Получится пустой список:

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Выберите наиболее удачный вариант реализации  `PushFront`  для шаблона  `SingleLinkedList`. Учтите, что элемент типа  `Type`  может выбрасывать исключение в самых неожиданных случаях. Например, при присваивании.

-   **(+)**  **Вариант 1**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

> Это самое простое решение. К тому же оно безопасно при возникновении исключений. Если во время работы оператора  `new`  будет выброшено исключение при выделении памяти либо при конструировании объекта, объект не создастся, а память вернётся обратно в кучу. Последующее присваивание указателя  `head_.next_node`  и инкремент  `size_`  исключений не выбрасывают.

-   **(-)**  **Вариант 2**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        front_node->value = value;
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    
private:
    Node head_;
    size_t size_ = 0;
};

```

> Этот код будет работать корректно, но только если нет исключений.  `Type`  может быть любым типом, даже способным выбросить исключение при присваивании, — например  `string`. Если  `front_node->value = value;`  выбросит исключение, память, выделенная ранее через  `new`, не освободится. Произойдёт утечка памяти.

-   **(-)**  **Вариант 3**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (...) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

> Это решение будет корректно работать как при отсутствии исключений, так и при их выбрасывании в операции  `new`  или в случае присваивания  `front_node->value = value;`. Но оно громоздкое и трудное в сопровождении.

-   **(-)**  **Вариант 4**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (const std::bad_alloc&) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
        ++size_;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

> Решение считалось бы верным, если бы операция присваивания могла выбросить только исключение  `std::bad_alloc`. Но параметром шаблона  `Type`  может выступать любой, в том числе пользовательский, тип. В этом случае следует учесть, что операция присваивания способна выбросить исключение любого типа. Обработчик  `bad_alloc`  это исключение не поймает. Выделенная операцией  `new`  память утечёт. Кроме того, решение выглядит громоздким.

-   **(-)**  **Вариант 5**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        Node* front_node = new Node();
        try {
            front_node->value = value;
        } catch (...) {
            delete front_node;
            throw;
        }
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

> Поле  `size_`  модифицируется до выполнения кода, способного выбросить исключение. Если операция  `new`  или присваивание  `value`  выбросят исключение, элемент в список не добавится, а значение поля  `size_`  не будет соответствовать реальному положению дел. Целостность данных контейнера нарушится. Это наверняка приведёт к проблемам в его работе.

-   **(-)**  **Вариант 6**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        Node* front_node = new Node();
        front_node->value = value;
        front_node->next_node = head_.next_node;
        head_.next_node = front_node;
    }
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

```

> Код станет неустойчивым к исключениям. Если операция  `new`  выбросит исключение, поле  `size_`  будет содержать размер, на единицу превышающий количество элементов списка. Данные списка окажутся несогласованными, и программа может начать работать некорректно. Кроме того, в роли параметра  `Type`  может быть тип, чья операция присваивания способна выбросить исключение, например  `string`. Если исключение будет выброшено во время присваивания  `value`, выделенная оператором  `new`  память утечёт.

-   **(-)**  **Вариант 7**

```cpp
template <typename Type>
class SingleLinkedList {
    ...
    void PushFront(const Type& value) {
        ++size_;
        head_.next_node = new Node(value, head_.next_node);
    }
private:
    Node head_;
    size_t size_ = 0;
};

```

> Инкремент поля  `size_`  происходит до вставки элемента в начало списка. Если операция  `new`  выбросит исключение, состояние списка окажется рассогласованным — значение поля  `size_`  не будет равно фактическому количеству элементов. Дальнейшее использование этого списка, скорее всего, приведёт к некорректной работе программы.


</details>

<details>
<summary>Задание:</summary>

## Задание

Реализуйте операцию  `PushFront`, которая делает вставки элемента в начало односвязного списка, и операцию  `Clear`, очищающую список.

Метод  `PushFront`  должен предоставлять строгую гарантию безопасности исключений: если в процессе работы метода будет выброшено исключение, состояние списка должно быть таким же, как до вызова метода.

Метод  `Clear`  очищает список и не должен выбрасывать исключений. Считается, что деструктор типа  `Type`  не выбрасывает исключений.

При разрушении списка должны быть удалены все его элементы.

Сигнатура методов:

```cpp
template <typename Type>
class SingleLinkedList {
public:
    ...
    // Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value) {
        // Реализуйте метод самостоятельно
    }

    // Очищает список за время O(N)
    void Clear() noexcept {
        // Реализуйте метод самостоятельно
    }
    ...
};

```

Пример использования и тесты методов  `PushFront`,  `Clear`  и деструктора  `SingleLinkedList`:

```cpp
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    ...
};

// Эта функция тестирует работу SingleLinkedList
void Test1() {
    // Шпион, следящий за своим удалением
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instance_counter) noexcept
            : instance_counter_ptr_(&instance_counter)  //
        {
            OnAddInstance();
        }
        DeletionSpy(const DeletionSpy& other) noexcept
            : instance_counter_ptr_(other.instance_counter_ptr_)  //
        {
            OnAddInstance();
        }
        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
            }
            return *this;
        }
        ~DeletionSpy() {
            OnDeleteInstance();
        }

    private:
        void OnAddInstance() noexcept {
            if (instance_counter_ptr_) {
                ++(*instance_counter_ptr_);
            }
        }
        void OnDeleteInstance() noexcept {
            if (instance_counter_ptr_) {
                assert(*instance_counter_ptr_ != 0);
                --(*instance_counter_ptr_);
            }
        }

        int* instance_counter_ptr_ = nullptr;
    };

    // Проверка вставки в начало
    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);

        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());

        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }

    // Проверка фактического удаления элементов
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});

            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);

            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);
        assert(item1_counter == 0);
        assert(item2_counter == 0);
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

    {
        bool exception_was_thrown = false;
        // Последовательно уменьшаем счётчик копирований до нуля, пока не будет выброшено исключение
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // Создаём непустой список
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // Если метод не выбросил исключение, список должен перейти в новое состояние
                assert(list.GetSize() == 2);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // После выбрасывания исключения состояние списка должно остаться прежним
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
}

int main() {
    Test1();
}

```

### Что отправлять на проверку

Только код с шаблоном класса  `SingleLinkedList`  и нужными для его работы директивами  `#include`. Если отправите функцию  `main`, она будет заменена на версию из тренажёра.

### Как будет тестироваться ваш код

Сохраните сигнатуры всех публичных методов класса  `SingleLinkedList`  неизменными, чтобы код скомпилировался без ошибок.

### Подсказка

-   Если вам сложно реализовать методы  `PushFront`  и  `Clear`, вернитесь к теории урока. Тесты и пример использования из условия помогут реализовать их корректно.
-   При разрушении списка вызовите в деструкторе метод  `Clear`, чтобы освободить память, занимаемую элементами списка.
-   В методе  `Clear`  удаляйте узлы, начиная с первого элемента, до тех пор, пока список не опустеет. Не забудьте обнулить поле  `size_`  по окончании зачистки.

</details>

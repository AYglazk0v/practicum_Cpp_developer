<details>
<summary>Теория:</summary>

# Копирование объектов. Часть вторая

Из предыдущего урока вы узнали о трудностях копирования объектов, содержащих указатели на динамическую память. Когда процесс не сводится к простому копированию полей объекта, программисту нужно создать копирующий конструктор для правильного выполнения этой операции.

### Выбираем способ хранить щупальца

Код осьминога из предыдущего урока был искусственно усложнён тем, что щупальца хранились в динамической памяти. Так вы смогли увидеть трудности своими глазами. Если бы в  `tentacles_`  хранились сами щупальца, а не указатели на них, проблем при копировании осьминога не возникло бы — все его щупальца скопировались бы автоматически.

Чтобы хранение указателей на щупальца вместо самих щупалец было более обоснованным, немного усложним задачу:

-   щупальце может прицепляться к произвольному щупальцу любого осьминога и отцепляться от него;
-   осьминог при создании может иметь произвольное количество щупалец. По умолчанию их восемь;
-   разрешается добавлять осьминогу новые щупальца, используя метод  `AddTentacle`.

```cpp
class Tentacle {
public:
    explicit Tentacle(int id) noexcept;
    int GetId() const noexcept;

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

class Octopus {
public:
    Octopus();
    explicit Octopus(int num_tentacles);

    void AddTentacle();
    size_t GetTentacleCount() const noexcept;
    const Tentacle& GetTentacle(size_t index) const;
    Tentacle& GetTentacle(size_t index);

    ~Octopus();

private:
    void Cleanup() noexcept;

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus1;
    Octopus octopus2;

    // Два осьминога прицепляются друг к другу щупальцами
    octopus1.GetTentacle(1).LinkTo(octopus2.GetTentacle(3));
    octopus2.AddTentacle();
    octopus2.GetTentacle(octopus2.GetTentacleCount() - 1).LinkTo(octopus1.GetTentacle(0));
}

```

Из-за возможности добавлять осьминогу новые щупальца, а щупальцам — ссылаться друг на друга, хранение щупалец в векторе становится затруднительным:

-   При добавлении нового щупальца ранее существовавшие ссылки на щупальца станут невалидны из-за особенностей реализации вектора. Почему так происходит, вы узнаете через несколько уроков.
-   Другие контейнеры могли сохранить валидность ссылок при вставке и удалении элементов, но не справились бы с неожиданным требованием разрешить осьминогам обмениваться щупальцами.

Храня щупальца в динамической памяти, делаем их более независимыми: их адрес не изменится ни при модификации контейнера, ни при передаче другому осьминогу. На практике такая задача может возникать регулярно — например, папки хранят наборы файлов, а гостиницы — наборы постояльцев.

### Анализируем код конструктора по умолчанию

Прежде чем начать писать конструктор копирования класса «‎Осьминог», взгляните на конструктор по умолчанию в классе  `Octopus`:

```cpp
class Octopus {
public:
    Octopus() {
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);      // Может выбросить исключение bad_alloc
                tentacles_.push_back(t);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
                // чтобы не удалить его в обработчике catch повторно
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер tentacles_
            Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в tentacles_
            delete t;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }
    ...
};

```

Код конструктора получился довольно сложным, так как нужно обработать исключение  `bad_alloc`. Оно может возникнуть при создании щупальца в динамической памяти.

Обработчик исключения должен удалить объекты, на которые ссылаются указатели в векторе  `tentacles_`. Удалить нужно и щупальце по адресу в переменной  `t`. Оно ещё не успело попасть в контейнер  `tentacles_`.

Если в таком же стиле написать конструктор копирования, он будет не менее сложным. Вот был бы способ автоматически удалять объект из динамической памяти, когда указатель, хранящий его адрес, выходит из своей области видимости!

В C++ эту задачу решают умные указатели — классы, которые благодаря перегрузке операций ведут себя как указатели. Они предоставляют операцию разыменования  `*`  и доступа к членам класса  `->`. В отличие от обычных указателей, которые ещё называют «сырыми»‎, умные указатели реализуют семантику владения объектом, то есть автоматически удаляют объект при наступлении определённых условий. Умные указатели также определяют, что должно происходить при копировании указателя. Как правило, умные указатели делают шаблонными, чтобы использовать с объектами разных типов.

В стандартной библиотеке есть классы умных указателей, которые упростили бы решение задачи. Вы обязательно с ними познакомитесь в следующих спринтах. В этом уроке вы создадите свой умный указатель, чтобы потом упростить класс  `Octopus`.

Умный указатель в качестве одного из своих полей хранит обычный указатель, а «‎умное поведение»‎ реализуется за счёт конструктора, деструктора и специфичных для указателя операций.

Ваш указатель  `ScopedPtr`  будет владеть объектом, созданным в куче, и гарантировать, что объект автоматически удалится при выходе из области видимости указателя.

В повседневной разработке вам вряд ли понадобится писать собственные умные указатели и вручную вызывать  `new`/`delete`. Скорее всего, использовать вы будете стандартные. Но задания позволят разобраться, как удобные стандартные классы устроены на самом деле, и чего от них ожидать.

</details>

<details>
<summary>Задание 1:</summary>

## Задание 1

У вас есть каркас шаблонного класса  `ScopedPtr`. Это умный указатель, в котором частично реализован такой функционал:

-   Конструктор по умолчанию инициализирует указатель нулевым значением;
-   Заготовка метода  `GetRawPtr`. Метод возвращает адрес объекта, на который ссылается умный указатель;
-   Заготовка конвертирующего конструктора для создания  `ScopedPtr`  из сырого указателя;
-   Удалённый конструктор копирования. В этом случае создать копию умного указателя будет нельзя. Как следствие, не будет проблем, когда два умных указателя владеют одним объектом.

```cpp
template <typename T>
class ScopedPtr {
public:
    ScopedPtr() = default;
    explicit ScopedPtr(T* raw_ptr) noexcept {...}
    ScopedPtr(const ScopedPtr&) = delete;
    ~ScopedPtr() {...}
    T* GetRawPtr() const noexcept {...}
    T* Release() noexcept {...}

private:
    T* ptr_ = nullptr;
};

```

Объект класса  `ScopedPtr<T>`  может находиться в двух состояниях: владеть объектом типа  `T`  или не владеть. При этом должны выполняться следующие правила:

-   Конструктор без параметров создаёт умный указатель, не владеющий объектом.
-   Конструктор  `ScopedPtr(T* raw_ptr)`  создаёт умный указатель, который владеет объектом, если и только если переданный указатель не был равен  `nullptr`.
-   Метод  `GetRawPtr`  не влияет на владение.
-   Метод  `Release`  отменяет владение, если оно было.

Цель умного указателя — удалить объект, которым он владеет. Он должен делать это в своём деструкторе, который вам предстоит реализовать вместе с недостающими методами.

Проверки в заготовке кода упростят реализацию конструкторов и деструкторов.

### Требования

Реализуйте недостающие методы класса  `ScopedPtr`:

-   Недостающий код конструктора  `ScopedPtr`  из «сырого»‎ указателя на объект в динамической памяти. Созданный  `ScopedPtr`  должен владеть переданным объектом. Допустимо значение указателя  `nullptr`. В этом случае владения не возникает.
-   Тело метода  `GetRawPtr`.
-   Удаление объекта, которым владеет  `ScopedPtr`, в деструкторе.
-   Тело метода  `Release`.

### Ограничения

Сохраните сигнатуры всех публичных методов класса  `ScopedPtr`  неизменными, чтобы код скомпилировался без ошибок.

### Что отправлять на проверку

Загрузите в тренажёр только код с шаблоном класса  `ScopedPtr`  и нужными для его работы директивами  `#include`. Функция  `main`  при проверке учитываться не будет.

### Как будет тестироваться ваш код

Шаблон класса  `ScopedPtr`  будет протестирован кодом, подобным тому, что есть в функции  `main`.

Передаваемые в конструктор указатели ссылаются на объект, созданный в куче с использованием операции  `new`, либо равны  `nullptr`.

### Подсказка

-   Используйте проверки в функции  `main`  из заготовки кода.
-   В конвертирующем конструкторе сохраните переданный «‎сырой»‎ указатель в поле  `ptr_`.
-   В методе  `GetRawPtr`  верните текущее значение  `ptr_`.
-   Удалите объект, на который ссылается поле  `ptr_`  в деструкторе.
-   В методе  `Release`  обнулите поле  `ptr_`, не удаляя сам объект, и верните прежнее значение поля.

</details>

<details>
<summary>Задание 2:</summary>

## Задание 2

Вы сделали класс  `ScopedPtr`  умным. Осталось превратить его в указатель.

Переопределите операции разыменования  `*`  и доступа к полям класса  `->`. При попытке вызвать их у нулевого указателя эти операции должны выбрасывать исключение  `std::logic_error`, а не приводить к неопределённому поведению. Всё-таки это умный указатель. Перегруженная операция приведения к типу  `bool`  облегчает проверку на  `nullptr`.

В заготовке кода — ваше решение предыдущей задачи. На его основе реализуйте недостающие операции.

Сигнатура новых операций умного указателя:

```cpp
// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
public:
    //...

    // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
    // на какой-либо объект
    explicit operator bool() const noexcept {
        // Реализуйте самостоятельно
    }

    // Оператор разыменования возвращает ссылку на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T& operator*() const {
        // Реализуйте самостоятельно
    }

    // Оператор -> должен возвращать указатель на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T* operator->() const {
        // Реализуйте самостоятельно
    }

    // ...

private:
    T* ptr_ = nullptr;
};

```

Оператор преобразования к типу  `bool`  объявлен  `explicit`, чтобы запретить неявное преобразование указателя в  `bool`, которое практически всегда нежелательно. На проверке указателя в условии  `if`  это не отразится.

```cpp
void Fn(int i);

int main() {
    ScopedPtr<Object> p(new Object());
    Fn(p); // Когда operator bool объявлен explicit, здесь будет ошибка компиляции
           // Без explicit в Fn будет передано значение 1
}

```

Пример использования  `ScopedPtr`  и тесты, которые помогут вам решить задачу, даны ниже.

```cpp
#include <cassert>

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
    ...
};

// Этот main тестирует класс ScopedPtr
int main() {
    // Проверка работы оператора приведения к типу bool
    {
        // Для нулевого указателя приведение к типу bool возвращает false
        const ScopedPtr<int> empty_ptr;
        assert(!empty_ptr);

        // Для ненулевого указателя приведение к типу bool возвращает true
        const ScopedPtr<int> ptr_to_existing_object(new int(0));
        assert(ptr_to_existing_object);

        static_assert(noexcept(static_cast<bool>(ptr_to_existing_object)));
    }

    // Проверка работы оператора разыменования *
    {
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
        // Ссылка, возвращаемая оператором разыменования, должна ссылаться на объект,
        // на который указывает умный указатель
        assert(&*smart_ptr == raw_ptr);

        try {
            ScopedPtr<int> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            *empty_ptr;
            // Сюда попасть мы не должны
            assert(false);
        } catch (const logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Проверка работы оператора ->
    {
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
        // Доступ к членам класса через умный указатель должен быть аналогичен
        // доступу через "сырой" указатель
        assert(smart_ptr->data() == raw_ptr->data());

        try {
            ScopedPtr<string> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            empty_ptr->clear();
            // Сюда попасть мы не должны
            assert(false);
        } catch (const logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Пример использования
    {
        // На этой структуре будет проверяться работа умного указателя
        struct Object {
            Object() {
                cout << "Object is default constructed"s << endl;
            }
            void DoSomething() {
                cout << "Doing something"s << endl;
            }
            ~Object() {
                cout << "Object is destroyed"s << endl;
            }
        };

        // Сконструированный по умолчанию указатель ссылается на nullptr
        ScopedPtr<Object> empty_smart_ptr;
        // Перегруженный оператор приведения к типу bool вернёт false для пустого указателя
        assert(!empty_smart_ptr);

        ScopedPtr<Object> smart_ptr(new Object());
        // Перегруженный оператор bool вернёт true для указателя, ссылающегося на объект
        assert(smart_ptr);

        // Проверка оператора разыменования
        (*smart_ptr).DoSomething();
        // Проверка оператора доступа к членам класса
        smart_ptr->DoSomething();
    }
}

```

### Ограничения

Указатели, передаваемые в методы класса, либо ссылаются на объект, созданный в куче при помощи операции  `new`, либо равны  `nullptr`.

### Как и что отправлять на проверку

Только код с шаблоном класса  `ScopedPtr`  и необходимыми для его работы директивами  `#include`. Если отправите  `main`, она будет заменена на версию из тренажёра.

### Как будет тестироваться ваш код

Шаблон класса  `ScopedPtr`  будет протестирован кодом, подобным представленному в функции  `main`  из описания задания. Сохраните сигнатуры всех публичных методов класса  `ScopedPtr`  неизменными, чтобы код скомпилировался без ошибок.

### Подсказка

-   В операциях  `*`  и  `->`  сначала проверьте сырой указатель на равенство  `nullptr`  и выбросьте исключение  `logic_error`  в случае, когда эти операции вызваны у пустого  `ScopedPtr`. Если всё хорошо, верните ссылку или указатель на объект в зависимости от реализуемой операции.
-   В операции приведения к  `bool`  верните  `false`, если  `ScopedPtr`  ссылается на  `nullptr`, и  `true`  в ином случае.

</details>

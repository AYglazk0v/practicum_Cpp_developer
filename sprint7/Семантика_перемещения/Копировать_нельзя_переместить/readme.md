<details>
<summary>Теория + тестирование:</summary>

# Копировать нельзя переместить

В предыдущих уроках мы старались всячески избежать лишних копирований. Компилятор оптимизировал, а программист вызывал функцию  `move`  и подсовывал вместо постоянных объектов временные. В целом, можно сформулировать правило:

«Там, где возможно копирование, должно быть возможно и перемещение».

Но вы уже знаете, что при копировании вызываются специальные методы — конструктор копирования или оператор присваивания. Если вместо копирования требуется осуществить перемещение, компилятору нужны другие методы. В уроке изучим это подробнее.

### Запрет на копирование

Вернёмся к задаче о считалке Иосифа. В заготовке кода был класс, объекты которого нельзя скопировать, и вам пришлось решать задачу, используя перемещение. Разберёмся, как этот класс был устроен:

```cpp
struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

```

Вы уже знаете о конструкторе копирования и операторе присваивания. В их сигнатурах возникает специальное слово  `delete`:

```cpp
NoncopyableInt(const NoncopyableInt&) = delete;
NoncopyableInt& operator=(const NoncopyableInt&) = delete;

```

Так программист может сообщить компилятору, что конструктора копирования и оператора присваивания у класса не существует. Значит, объекты класса запрещено копировать.

Возможность запретить копирование важна, когда копирование объекта может привести к непредсказуемым или нежелательным последствиям. Примеры класса, где копирование запрещено —  `istream`  и  `ostream`. Потоки вывода и ввода должны управляться только из одного объекта. Если несколько объектов начнут пытаться писать или читать из потока, будет похоже на то, как несколько человек одновременно печатают на одной клавиатуре или читают вслух с разной скоростью один текст. При попытке копирования компилятор сообщит об ошибке:

```cpp
// попробуем скопировать cout в наш объект out
ostream out(cout);
out << "Неужели скопировалось?"s << endl;

```

```cpp
use of deleted function 'std::basic_ostream<_CharT, _Traits>::basic_ostream(const std::basic_ostream<_CharT, _Traits>&) [with _CharT = char; _Traits = std::char_traits<char>]'
     ostream out(cout);
In file included from C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/iostream:39,
                 from 1.cpp:1:
C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include/c++/ostream:391:7: note: declared here
       basic_ostream(const basic_ostream&) = delete;

```

После сигнатуры оператора копирования есть слово  `delete`, и компилятор не даёт скопировать объект.

### Стандартное перемещение

В примере класса из считалки Иосифа есть две сигнатуры, в которых пора разобраться:

```cpp
NoncopyableInt(NoncopyableInt&&) = default;
NoncopyableInt& operator=(NoncopyableInt&&) = default;

```

Перед вами конструктор перемещения и перемещающий оператор присваивания.

Начнём с простого. Специальное слово  `default`  говорит, что компилятор должен сам решить, как будет происходить перемещение. Часто это удобно. Компилятор просто применит перемещение к каждому члену класса. Если для члена класса определён конструктор перемещения, компилятор будет стараться его найти. Если хотя бы один из членов класса переместить нельзя, компилятор не справится с поставленной задачей и сообщит об ошибке. Вот что случится, если удалить перемещающий конструктор и оператор присваивания:

```cpp
#include <iostream>

using namespace std;

struct NonmovingInt {
    int value;
		
    // копировать, нельзя перемещать!
    NonmovingInt(const NonmovingInt&) = default;
    NonmovingInt& operator=(const NonmovingInt&) = default;

    NonmovingInt(NonmovingInt&&) = delete;
    NonmovingInt& operator=(NonmovingInt&&) = delete;
};

struct NoncopyableInt {
    // пусть членом класса станет неперемещаемый тип
    NonmovingInt value;
		
    // копировать нельзя, перемещать!
    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

int main() {
    NoncopyableInt source{1};
    NoncopyableInt target(move(source));
    cout << "Неужели перенеслось?"s << endl;
    return 0;
}

```

```cpp
example.cpp: In function 'int main()':
example.cpp:27:39: error: use of deleted function 'NoncopyableInt::NoncopyableInt(NoncopyableInt&&)'
     NoncopyableInt target(move(source));
                                       ^
example.cpp:21:5: note: 'NoncopyableInt::NoncopyableInt(NoncopyableInt&&)' is implicitly deleted because the default definition would be ill-formed:
     NoncopyableInt(NoncopyableInt&&) = default;
     ^~~~~~~~~~~~~~
example.cpp:21:5: error: use of deleted function 'NonmovingInt::NonmovingInt(NonmovingInt&&)'
example.cpp:11:5: note: declared here
     NonmovingInt(NonmovingInt&&) = delete;

```

С простым разобрались, перейдём к интересному. Тип принимаемого значения в конструкторе копирования отличается от конструктора перемещения. Вместо  `const NoncopyableInt&`  при перемещении конструктор получает  `NoncopyableInt&&`. Это тоже ссылка на объект типа  `NoncopyableInt`, но необычная. Не ссылка на ссылку, а специальная rvalue-ссылка. Она относится к объекту, к адресу которого мы не можем получить прямой доступ. Такие ссылки бывают на временные объекты. Вызывая функцию  `move`  для обычного объекта, просто приводим его к типу rvalue-ссылки. rvalue-cсылки позволяют компилятору разобраться, нужно ли ему вызывать конструктор копирования или конструктор перемещения. Так компилятор отличает временные объекты от постоянных.

### Перемещение под контролем

Программисту бывает недостаточно стандартной реализации конструкторов и операторов присваивания — как копирующих, так и перемещающих. В таких случаях он может определить эти методы самостоятельно. Полезной функцией для конструктора перемещения и перемещающего оператора присваивания оказывается  [`exchange`](https://en.cppreference.com/w/cpp/utility/exchange)  из библиотеки  `<utility>`.

----------

Прочитайте о функции  `exchange`  или попробуйте догадаться, что будет выведено в результате:

```cpp
int y = 1;
int z = 2;
int x = exchange(y, z);
cout << x << " " << y << " " << z << endl;

```

-   1 2 2
    
-   1 1 2
    
-   1 2 1
    
-   Всё поменяется местами, но как конкретно — неизвестно.
    

Часто функцией  `exchange`  пользуются, чтобы гарантировать определённое значение в перемещаемом объекте после собственно перемещения.

Спрячем кролика в шляпу:

```cpp
class Rabbit {
public:
    enum class Color { WHITE, BLACK };

    Rabbit(Color color)
        : color_(color) 
    {
    }
    // копирование кроликов, как и слонов, в природе не предусмотрено
    Rabbit(const Rabbit& other) = delete;
    Rabbit& operator=(const Rabbit& other) = delete;

    Rabbit(Rabbit&& other) {
        ???
    }
    Rabbit& operator=(Rabbit&& other) {
        ???
        return *this;
    }

    Color GetColor() const {
        return color_;
    }

private:
    Color color_;
};

class Hat {
public:
    Hat(Rabbit&& rabbit)
        : rabbit_(move(rabbit)) 
    {
    }
    // не стоит копировать шляпу
    Hat(const Hat& other) = delete;
    Hat& operator=(const Hat& other) = delete;
    // но вот переместить шляпу можно
    Hat(Hat&& other) = default;
    Hat& operator=(Hat&&) = default;
    // в любой момент можно посмотреть, есть ли кролик в шляпе
    const Rabbit& GetRabbit() const {
        return rabbit_;
    }
private:
    Rabbit rabbit_;
};

```

Допустим, нужно переместить шляпу с белым кроликом внутри:

```cpp
Hat magic_hat(Rabbit(Rabbit::Color::WHITE));
Hat other_magic_hat(move(magic_hat));

```

Продвинутый фокусник хотел бы знать, что случится с кроликом в шляпе  `magic_hat`. Конкретно его интересует цвет животного. Посмотрим, что будет, если оставить перемещающие методы на усмотрение компилятору:

```cpp
class Rabbit {
    ...
    Rabbit(Rabbit&& other) = default;
    Rabbit& operator=(Rabbit&& other) = default;
    ...
}
...
int main() {
    Hat magic_hat(Rabbit(Rabbit::Color::WHITE));
    Hat other_magic_hat(move(magic_hat));
    if (magic_hat.GetRabbit().GetColor() == Rabbit::Color::WHITE) {
        cout << "Кролик перемещён, но его цвет белый"s << endl;
    }
    else if (magic_hat.GetRabbit().GetColor() == Rabbit::Color::BLACK) {
        cout << "Кролик перемещён, но его цвет чёрный"s << endl;
    }
    else {
        cout << "Кажется, мы не уверены на счёт цвета кролика"s << endl;
    }
}

```

Эта программа сообщит:

```cpp
Кролик перемещён, но его цвет белый

```

Кролик переместился, белый цвет остался. Для программиста ничего страшного не произошло. Объект остался в валидном состоянии. Но для фокусника этот результат выглядит странно. Хорошим тоном будет задать значение цвета, которое объект будет получать по умолчанию после перемещения. Пусть перемещённый кролик становится невидимым благодаря функции  `exchange`:

```cpp
Rabbit(Rabbit&& other) {
    color_ = exchange(other.color_, Color::INVISIBLE);
}

Rabbit& operator=(Rabbit&& other) {
    color_ = exchange(other.color_, Color::INVISIBLE);
    return *this;
}

```

Теперь точно известно, что в  `magic_hat`  после перемещения будет лежать невидимый кролик.

В итоговом проекте спринта вы будете добавлять поддержку move-семантики для своего вектора и списка. Функция  `exchange`  будет полезна для гарантии, что указатель на данные не остался в перемещённом объекте, а был заменён на  `nullptr`.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Прочитайте о функции  `exchange`  или попробуйте догадаться, что будет выведено в результате:

```cpp
int y = 1;
int z = 2;
int x = exchange(y, z);
cout << x << " " << y << " " << z << endl;

```

-   **(+)**  1 2 2
    
-   **(-)**  1 1 2
    
-   **(-)**  1 2 1
    
-   **(-)**  Всё поменяется местами, но как конкретно — неизвестно.

</details>

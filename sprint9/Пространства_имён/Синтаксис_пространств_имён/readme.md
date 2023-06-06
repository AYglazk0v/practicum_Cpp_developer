<details>
<summary>Теория + тестирование :</summary>

# Синтаксис пространств имён

Пространства имён помогли избежать конфликта и использовать две похожие библиотеки в одной программе. В этом уроке разберём все тонкости их использования.

### Создание и расширение пространств имён

У пространств имён есть особенность, отличающая их от других сущностей C++: одно и то же пространство имён может определяться сколько угодно раз в разных частях программы. Так, в прошлом уроке мы писали  `namespace xml`  в файле  `xml.h`  и  `namespace xml`  в файле  `xml.cpp`, но конфликта при этом не возникло. Можно добавить ещё одно имя в это пространство, написав в  `main.cpp`  такой код:

```cpp
...

namespace xml {

Document LoadSpendings(istream& in) {
    // ...
}

}

```

Это синтаксически верно, но делать так не рекомендуется — пространство имён  `xml`  — прерогатива библиотеки XML, а не использующего её приложения. Если вы в своём приложении добавите класс или функцию в пространство имён  `xml`, то при обновлении библиотеки рискуете столкнуться с конфликтом имён.

### Вложенные пространства имён

Пространства имён могут быть вложены друг в друга. Например, принято выделять вспомогательные функции и классы библиотеки во вложенное пространство имён  `detail`. Его можно использовать так:

```cpp
// файл json.cpp

#include "json.h"

using namespace std;

namespace json {
...
// вложенное пространство имён
// его имя с квалификацией json::detail
namespace detail {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    ...
}

Node LoadInt(istream& input) {
    ...
}

Node LoadString(istream& input) {
    ...
}

Node LoadDict(istream& input) {
    ...
}

Node LoadNode(istream& input) {
    ...
}

// завершаем пространство имён json::detail
}

Document Load(istream& input) {
	    return Document{detail::LoadNode(input)};
}

// завершаем пространство имён json
}

```

В пространстве имён  `detail`  обычно размещают сущности, которые предназначены для внутреннего использования. Эти детали реализации могут изменяться от версии к версии, в то время как сама библиотека будет иметь стабильный программный интерфейс. В коде, использующем библиотеку, не рекомендуется ссылаться на сущности из  `detail`, чтобы после обновления библиотеки не столкнуться с ошибками.

----------

Какое будет полное квалифицированное имя у функции  `g`  из следующего примера?

```cpp
namespace A {

namespace B {
void f();
}

namespace C {
namespace D {
void g();
}
}

}

```

Посмотрите внимательно, в какие пространства имён попадает  `g`.

Правильный ответ  `A::C::D::g`. Пространство  `B`  закрылось раньше.

----------

### Определения методов и функций

Функции и методы, объявленные внутри пространства имён, можно определять двумя способами:

1.  внутри конструкции  `namespace`,
2.  вне конструкции  `namespace`, явно указывая квалификацию каждого имени.

Когда применяется первый способ, определение целиком помещается в конструкцию  `namespace`  и квалифицировать имена того же пространства не нужно: при поиске сущности по имени компилятор обязательно будет принимать во внимание все видимые имена этого пространства.

Второй способ позволяет определить объявленную внутри конструкции  `namespace`  функцию или метод, не прибегая к этой конструкции второй раз.

Вот как определяется функция  `Load`  во втором случае:

```cpp
json::Document json::Load(istream& input) {
    return Document{LoadNode(input)};
}

```

Пришлось квалифицировать не только имя функции, но и тип возвращаемого значения. Внутри реализации имена квалифицировать необязательно, так как тело функции уже относится к её пространству имён. Также необязательно квалифицировать имена в списке параметров.

В отличие от определений, объявления функций и классов можно делать только первым способом — внутри конструкции  `namespace`.

Изучение синтаксиса пространств имён на этом завершается. Чтобы закрепить тему, ответьте на вопрос и решите задачу.

----------

Посмотрите на пример кода:

```cpp
// buildings.h

#include <vector>

namespace buildings {
struct Materials {};

Materials GetMaterials();

class Skyscraper {
    struct Floor {
        /* 1 */ buildings::Materials materials;
        int number;
    };

    Floor BuildFloor(/* 2 */ buildings::Materials materials);
    void BuildRoof(buildings::Materials materials);
    int GetMaterials(buildings::Materials source);

    bool IsComplete() const {
        return static_cast<int>(floors_.size()) < total_floors_;
    }

public:
    void Build() {
        while (!IsComplete()) {
            auto materials = /* 3 */ buildings::GetMaterials();
            floors_.push_back(BuildFloor(std::move(materials)));
        }
    }

private:
    int total_floors_ = 100;
    std::vector<Floor> floors_;
    /* 4 */ buildings::Materials roof_;
};
}  // namespace buildings

// buildings.cpp
#include "buildings.h"

using namespace std;

/* 5 */ buildings::Skyscraper::Floor /* 6 */ buildings::Skyscraper::BuildFloor(/* 7 */ buildings::Materials materials) {
    /* 8 */ buildings::Materials materials_copy = move(materials);
    return Floor{move(materials_copy), static_cast<int>(floors_.size())};
}

namespace buildings {
void /* 9 */ buildings::Skyscraper::BuildRoof(/* 10 */ buildings::Materials materials) {
    /* 11 */ buildings::Materials materials_copy = move(materials);
    roof_ = move(materials_copy);
}
}  // namespace buildings

```

В каких местах квалификацию  `bildings::`  можно опустить? В случае сомнений проверяйте правильность программы компилятором.

-   1
    
-   2
    
-   3
    
-   4
    
-   5
    
-   6
    
-   7
    
-   8
    
-   9
    
-   10
    
-   11

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

----------

Какое будет полное квалифицированное имя у функции  `g`  из следующего примера?

```cpp
namespace A {

namespace B {
void f();
}

namespace C {
namespace D {
void g();
}
}

}

```

Посмотрите внимательно, в какие пространства имён попадает  `g`.

----------

Посмотрите на пример кода:

```cpp
// buildings.h

#include <vector>

namespace buildings {
struct Materials {};

Materials GetMaterials();

class Skyscraper {
    struct Floor {
        /* 1 */ buildings::Materials materials;
        int number;
    };

    Floor BuildFloor(/* 2 */ buildings::Materials materials);
    void BuildRoof(buildings::Materials materials);
    int GetMaterials(buildings::Materials source);

    bool IsComplete() const {
        return static_cast<int>(floors_.size()) < total_floors_;
    }

public:
    void Build() {
        while (!IsComplete()) {
            auto materials = /* 3 */ buildings::GetMaterials();
            floors_.push_back(BuildFloor(std::move(materials)));
        }
    }

private:
    int total_floors_ = 100;
    std::vector<Floor> floors_;
    /* 4 */ buildings::Materials roof_;
};
}  // namespace buildings

// buildings.cpp
#include "buildings.h"

using namespace std;

/* 5 */ buildings::Skyscraper::Floor /* 6 */ buildings::Skyscraper::BuildFloor(/* 7 */ buildings::Materials materials) {
    /* 8 */ buildings::Materials materials_copy = move(materials);
    return Floor{move(materials_copy), static_cast<int>(floors_.size())};
}

namespace buildings {
void /* 9 */ buildings::Skyscraper::BuildRoof(/* 10 */ buildings::Materials materials) {
    /* 11 */ buildings::Materials materials_copy = move(materials);
    roof_ = move(materials_copy);
}
}  // namespace buildings

```

В каких местах квалификацию  `bildings::`  можно опустить? В случае сомнений проверяйте правильность программы компилятором.

-   **(+)**  1

> Имя из текущего пространства имён квалифицировать не нужно.

-   **(+)**  2

> Имя из текущего пространства имён квалифицировать не нужно.

-   **(-)**  3

> Имя конфликтует с методом  `GetMaterials`. Имя следует квалифицировать, чтобы показать, что нужно имя не из класса. Несмотря на то, что у функций разные параметры, перегрузка тут не сработает, так как в перегрузке могут участвовать только функции одной области видимости.

-   **(+)**  4

> Имя из текущего пространства имён квалифицировать не нужно.

-   **(-)**  5

> Вне пространства имён нет имени  `Skyscraper`. В типе возвращаемого значения его тоже нужно квалифицировать.

-   **(-)**  6

> Вне пространства имён нет имени  `Skyscraper`, его нужно квалифицировать.

-   **(+)**  7

> Параметры относятся к области видимости функции или метода. Квалифицировать их типы необязательно.

-   **(+)**  8

> Вся функция находится в пространстве имён  `building`, её тело и список параметров относятся к нему же, несмотря на то, что формально они находятся вне конструкции  `namespace buildings { ... }`.

-   **(+)**  9

> Имя из текущего пространства имён квалифицировать не нужно.

-   **(+)**  10

> Имя из текущего пространства имён квалифицировать не нужно.

-   **(+)**  11

> Имя из текущего пространства имён квалифицировать не нужно.

</details>

<details>
<summary>Задание:</summary>

## Задание

В прошлых уроках вы работали с простыми библиотеками для чтения JSON и XML. Теперь вам предлагается разработать похожую библиотеку самостоятельно. Она будет читать ini-файлы.

Не совершайте нашу ошибку и сразу оформите библиотеку в отдельном пространстве имён. Назовите его  `ini`.

Интерфейс библиотеки — в заготовке кода. Но включить его в пространство имён вам нужно будет самостоятельно.

ini-файл состоит из секций. Секция имеет имя и содержит пары строк — ключ, значение. Секция задаётся типом  `Section`, являющимся псевдонимом для  `unordered_map<string, string>`. Реализуйте функцию  `ini::Load`, читающую ini-файл из потока и возвращающую документ — класс  `ini::Document`, где нужно реализовать следующие методы:

-   `Section& AddSection(std::string name)`  — добавляет секцию с указанным именем и возвращает ссылку на неё.
-   `const Section& GetSection(const std::string& name) const`  — возвращает секцию по имени. В случае пустой или несуществующей секции возвратите пустой словарь.
-   `std::size_t GetSectionCount() const`  — возвращает количество секций в ini-файле. Следует учитывать в том числе пустые секции.

### Пример

Пример можете найти в функции  `main`  заготовки кода.

### Ограничения

-   Не меняйте  `main.cpp`.
-   Класс  `Document`  и функция  `Load`  должны находится в пространстве имён  `ini`.
-   Определения методов  `Document`  и функции  `Load`  должны находится в ini.cpp.

**Ограничения входных данных**

На вход будет передаваться корректный ini-файл, который удовлетворяет следующим ограничениям:

-   Каждая строка — имя секции, присваивание либо пустая строка;
-   Пустая строка может содержать только пробелы, но в любом количестве;
-   Первая непустая строка — имя секции;
-   В имени секции первый непробельный символ это  `[`. Присваивание не может начинаться с этого символа;
-   В имени секции последний непробельный символ это  `]`. Всё, что между  `[`  и  `]`, это и есть имя секции;
-   Присваивание имеет вид  `<ключ>=<значение>`. Нужно игнорировать возможные пробелы в начале и конце строки, а также вокруг знака равно.
-   У двух секций не может совпадать имя.
-   Внутри секции не может быть двух одинаковых ключей.

### Как будет тестироваться ваш код

Мы проверим, что:

-   функция  `Load`  правильно читает корректные ini-файлы;
-   вы поместили нужную функцию и класс в  `namespace ini`;
-   класс  `Document`  работает правильно;
-   вы определяете функции и методы библиотеки в файле  `ini.cpp`.

### Подсказка

-   Вы можете поместить все определения в  `ini.cpp`  в конструкцию  `namespace ini {...}`, а можете каждый раз использовать квалифицированное имя. Первый способ сократит код.
-   Читайте поток построчно функцией  `std::getline`.
-   Вам пригодятся следующие методы класса  `string`:  `find_first_not_of`,  `find_last_not_of`,  `find`,  `substr`.

</details>

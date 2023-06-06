<details>
<summary>Теория + тестирование:</summary>

# Сокращаем с using

Разберём способы избежать квалификации. С одним из таких способов вы хорошо знакомы с первых уроков:  `using namespace std`. Он позволяет убрать квалификацию у имён стандартной библиотеки и вместо  `std::map<std::string, std::pair<std::string, std::optional<std::string_view>>>`  написать  `map<string, pair<string, optional<string_view>>>`. Это тоже не выглядит просто, но всё-таки короче и легче читается.

----------

Представьте себе такой диалог:

— Как дела у Пети Иванова?

— О, у него дела отлично!

— У Пети Иванова что-то новое?

— Да, Петя Иванов защитил диссертацию и уехал за границу.

— А в какую страну Петя Иванов уехал?

— В Голландию!

Есть в этом диалоге что-то странное и неестественное — Петю всё время называют по имени и фамилии. В реальной жизни так говорить не будут — фамилию назовут только в первый раз, если это необходимо, а дальше будут говорить просто «‎Петя», поскольку уже понятно, что имеется в виду именно Петя Иванов, а не Петров и не Сидоров. Если убрать фамилию в третьей и последующих строчках, диалог станет вполне естественным.

Но в C++ вне пространства имён приходится использовать полное квалифицированное имя. Так произошло, например, в следующем юнит-тесте для библиотеки INI:

```cpp
void TestDocument() {
    ini::Document doc;
    assert(doc.SectionCount() == 0u);

    ini::Section* section = &doc.AddSection("one"s);
    assert(doc.SectionCount() == 1u);

    section->insert({"name_1"s, "value_1"s});
    section->insert({"name_2"s, "value_2"s});

    section = &doc.AddSection("two"s);
    section->insert({"name_1"s, "value_1"s});
    section->insert({"name_2"s, "value_2"s});
    section->insert({"name_3"s, "value_3"s});

    section = &doc.AddSection("three"s);
    section->insert({"name_1"s, "value_1"s});

    assert(doc.SectionCount() == 3u);
    const ini::Section expected_one = {{"name_1"s, "value_1"s}, {"name_2"s, "value_2"s}};
    const ini::Section expected_two = {{"name_1"s, "value_1"s}, {"name_2"s, "value_2"s}, {"name_3"s, "value_3"s}};
    const ini::Section expected_three = {{"name_1"s, "value_1"s}};

    assert(doc.GetSection("one"s) == expected_one);
    assert(doc.GetSection("two"s) == expected_two);
    assert(doc.GetSection("three"s) == expected_three);
}

```

Обратите внимание, что квалифицировать имя  `Section`  пришлось несколько раз. При чтении кода было бы понятно и так, что работа производится с сущностями именно этой библиотеки, но компилятор требует квалификацию. Сейчас пространство имён короткое — его название состоит всего из трёх букв. А в реальных программах оно может достигать 20 и более символов. В таких случаях частое использование квалификации затрудняло бы чтение кода.

Чтобы сократить написание и упростить чтение, можно применить декларацию  `using`:

```cpp
using ini::Section;

```

Она добавит имя в область видимости, после чего  `Section`  можно будет использовать без квалификации. Код стал немного проще:

```cpp
void TestDocument() {
    ini::Document doc;
    assert(doc.SectionCount() == 0u);

    using ini::Section;
    Section* section = &doc.AddSection("one"s);
    assert(doc.SectionCount() == 1u);

    section->insert({"name_1"s, "value_1"s});
    section->insert({"name_2"s, "value_2"s});

    section = &doc.AddSection("two"s);
    section->insert({"name_1"s, "value_1"s});
    section->insert({"name_2"s, "value_2"s});
    section->insert({"name_3"s, "value_3"s});

    section = &doc.AddSection("three"s);
    section->insert({"name_1"s, "value_1"s});

    assert(doc.SectionCount() == 3u);
    const Section expected_one = {{"name_1"s, "value_1"s}, {"name_2"s, "value_2"s}};
    const Section expected_two = {{"name_1"s, "value_1"s}, {"name_2"s, "value_2"s}, {"name_3"s, "value_3"s}};
    const Section expected_three = {{"name_1"s, "value_1"s}};

    assert(doc.GetSection("one"s) == expected_one);
    assert(doc.GetSection("two"s) == expected_two);
    assert(doc.GetSection("three"s) == expected_three);
}

```

----------

Сколько квалификаций удалось убрать, добавив  `using`-декларацию? Введите ответ числом:

Нам удалось убрать четыре квалификации. Посчитаем, на сколько символов сократился код. 20 символов мы сэкономили, убрав  `ini::`  перед именем  `Section`, но 19 пришлось потратить на саму конструкцию  `using ini::Section;`. Итого — выигрыш всего в один символ.

Как видно, экономия небольшая, но код стало проще воспринимать. Похожая ситуация была в диалоге о Пете из начала урока. А читаемость кода — это гораздо более важный параметр, чем количество символов в нём.

----------

Такая  `using`-декларация распространяется только на то имя, которое указано в ней явно, и только на ту область видимости, где она объявлена.  `using`  можно написать и вне областей видимости, например в начале файла. Тогда имя станет глобальным — будет распространяться на весь файл. Это соответствует ситуации, когда человек недавно чего-нибудь достиг, и о нём все говорят. Использовать его имя можно, не уточняя, кому именно оно принадлежит.

Теперь представьте, что обсуждают членов одной семьи. Их называют по имени и фамилии. Довольно бессмысленное занятия — ведь фамилия у всех одинаковая. Так никто не делает. В C++ подобное возникает, когда вне пространства имён вам нужно использовать самые разные имена этого пространства.

Например, в другом юнит-тесте библиотеки используется три разных имени из  `ini`:

```cpp
void TestLoadIni() {
    istringstream input(
        R"([july]
food=2500
sport=12000
travel=23400
clothes=5200
[august]
food=3250
sport=10000
travel=0
clothes=8300
jewelery=25000
)"s);

    const ini::Document doc = ini::Load(input);

    assert(doc.SectionCount() == 2u);

    const ini::Section expected_july = {
        {"food"s, "2500"s},
        {"sport"s, "12000"s},
        {"travel"s, "23400"s},
        {"clothes"s, "5200"s},
    };

    const ini::Section expected_august = {
        {"food"s, "3250"s}, {"sport"s, "10000"s}, {"travel"s, "0"s}, {"clothes"s, "8300"s}, {"jewelery"s, "25000"s},
    };

    assert(doc.GetSection("july"s) == expected_july);
    assert(doc.GetSection("august"s) == expected_august);
}

```

Здесь имеет смысл использовать директиву  `using namespace`, которая сделает доступными все имена выбранного пространства имён. В области видимости, где эта декларация есть, можно использовать имена выбранного пространства имён без квалификации:

```cpp
void TestLoadIni() {
    istringstream input(
        R"([july]
food=2500
sport=12000
travel=23400
clothes=5200
[august]
food=3250
sport=10000
travel=0
clothes=8300
jewelery=25000
)"s);

    using namespace ini;

    const Document doc = Load(input);

    assert(doc.SectionCount() == 2u);

    const Section expected_july = {
        {"food"s, "2500"s},
        {"sport"s, "12000"s},
        {"travel"s, "23400"s},
        {"clothes"s, "5200"s},
    };

    const Section expected_august = {
        {"food"s, "3250"s}, {"sport"s, "10000"s}, {"travel"s, "0"s}, {"clothes"s, "8300"s}, {"jewelery"s, "25000"s},
    };

    assert(doc.GetSection("july"s) == expected_july);
    assert(doc.GetSection("august"s) == expected_august);
}

```

Код сократился, и читать его стало проще. Есть соблазн поставить директиву  `using namespace`  прямо в начало файла, как мы делаем это иногда с  `using namespace std`:

```cpp
...
#include "json.h"
#include "xml.h"

using namespace json;
using namespace xml;

```

Компилируем программу и видим ошибку —  `error: reference to 'Document' is ambiguous`.

Эти конструкции  `using namespace`  отменили все преимущества, которые дал сам  `namespace`, сделав фактически обратное действие. Мы вернулись к разбитому корыту — ситуации, когда у нас не было пространств имён, но мы всё равно хотели использовать JSON и XML одновременно. Однако между двумя этими случаями есть разница: в первом ошибка возникла сразу при попытке включения двух файлов, а во втором — только при использовании имени. Это делает ошибку более опасной, поскольку она выявляется не сразу.

Всё это диктует важное правило: использовать директиву  `using namespace`  нужно только в том месте, где она действительно необходима, максимально сужая область её действия.

Особенно неприятно, когда конструкция  `using namespace`  встречается в h-файле, причём её действие не ограничено областью видимости. В этом случае она будет распространяться на все файлы, которые используют этот заголовочный файл. Пользователи h-файла могут совсем не ожидать, что он будет делать видимыми имена какого-либо пространства имён, и это приведёт к конфликтам. Поэтому делать так ни в коем случае не нужно.

У декларации  `using namespace`  есть ещё один эффект. Если написать  `using namespace my_beautiful_namespace`  внутри другого пространства имён, оно примет все имена пространства  `my_beautiful_namespace`:

```cpp
#include <iostream>

namespace verbs {
void Clap() {
    std::cout << "Хлоп!" << std::endl;
}
}  // namespace verbs

namespace words {
using namespace verbs;
}

int main() {
    // обращаемся к функции verbs::Clap через пространство words
    words::Clap();
}

```

У декларации  `using`  и директивы  `using namespace`  есть некоторые различия, выходящие за рамки курса. О них можно почитать в  [документации](https://en.cppreference.com/w/cpp/language/namespace). Они касаются поведения при конфликте имён. Можно считать, что  `using`  работает чётко — помещает имя в текущую область видимости, и это имя будет всегда иметь приоритет при конфликтах над именами, расположенными в объемлющих областях. С  `using namespace`  так не происходит — приоритет при конфликтах эта конструкция даёт не всегда.

----------

Какие из этих примеров не содержат ошибки?

-   `using std::string;`
    
-   `using namespace std::chrono;`
    
-   `using namespace std::vector;`
    
-   `using namespace std::vector<int>;`
    
-   `using std::map<int, std::string>;`
    
-   `using namespace std; using namespace chrono;`
    
-   `using std;`

</details>

<details>
<summary>Задание:</summary>

# Ответы на задания

Сколько квалификаций удалось убрать, добавив  `using`-декларацию? Введите ответ числом:

----------

Какие из этих примеров не содержат ошибки?

-   **(+)**  `using std::string;`
    
-   **(+)**  `using namespace std::chrono;`
    
-   **(-)**  `using namespace std::vector;`
    

> `std::vector`  — квалифицированное имя класса, а не пространства имён, использовать в  `using namepsace`  его нельзя

-   **(-)**  `using namespace std::vector<int>;`

> `std::vector<int>`  уж точно не имя

-   **(-)**  `using std::map<int, std::string>;`

> `std::map<int, std::string>`  это не имя

-   **(+)**  `using namespace std; using namespace chrono;`
    
-   **(-)**  `using std;`
    

> Для пространств имён предназначена конструкция  `using namespace`.

</details>

<details>
<summary>Теория:</summary>

# Итераторы в конструкторах контейнеров

Итераторы широко применяются не только в библиотеке алгоритмов и циклах обхода, но и в методах контейнеров. В этом уроке посмотрим, как это работает в конструкторах, а в следующем уроке затронем методы вставки и удаления, использующие итераторы.

Применять итераторы в конструкторах бывает удобно. Из вектора языков программирования с повторениями мы можем с легкостью сделать множество уникальных языков:

```cpp
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename It>
auto MakeSet(It range_begin, It range_end) {
    return set(range_begin, range_end);
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s, "C++"s, "C++"s, "Ruby"s, "Java"s};
    auto unique_langs = MakeSet(langs.begin(), langs.end());
    PrintRange(unique_langs.begin(), unique_langs.end());
}
```

Результат работы программы:

```
C# C++ Java Python Ruby
```

Обратите внимание на простоту и изящество кода в примере. Мы просто даём итераторы — даже не сам контейнер — и компилятор понимает, какой конструктор для множества вызвать.

</details>

<details>
<summary>Задание:</summary>

### Задание

Напишите функцию  `MakeVector`, аналогичную функции  `MakeSet`  из урока. Она должна быть шаблоном, принимающим итераторы на начало и конец диапазона элементов и возвращающим полученный из этих элементов вектор.

### Пример использования

В результате работы функции получится вектор, содержащий все элементы из множества:

```cpp
set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
vector<string> langs = MakeVector(unique_langs.begin(), unique_langs.end());
PrintRange(langs.begin(), langs.end());
```

### Пример вывода

```
C# C++ Java Python Ruby
```

### Подсказка

Используйте пример из урока, где вызывается конструктор с параметрами-итераторами.

</details>

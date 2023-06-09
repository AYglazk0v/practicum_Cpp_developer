<details>
<summary>Теория + тестирование:</summary>

# Хеш-функции

Решая задачу в прошлом уроке, вы могли подумать, что кошелёк подходит для хранения любого типа данных:  `int64_t`  легко заменить на строки, бородатых гитаристов или  `bool`. Имея в распоряжении перечень всех возможных значений, можно было бы создать эффективный контейнер на основе вектора. Этот контейнер хранил бы количество объектов каждого вида.

Например, вместо номиналов можно использовать названия банкнот, задаваемые переменной  `string`. Единственное, что для этого нужно — определить функцию индексации, которая по объекту выдаст номер ячейки для его хранения. Так вы можете сохранять произвольный объект:

```cpp
template <typename T>
class MoneyBox {
public:
    MoneyBox(vector<T> nominals)
        : nominals_(move(nominals))
        , counts_(nominals_.size()) {
    }

    const vector<int>& GetCounts() const {
        return counts_;
    }

    int GetIndex(const T& value) const {
        return find(nominals_.begin(), nominals_.end(), value) - nominals_.begin();
    }

    void PushCoin(const T& value) {
        int pos = GetIndex(value);
        assert(pos < int(nominals_.size()));

        ++counts_[pos];
    }

    void PrintCoins(ostream& out) const {
        for (int i = 0; i < int(nominals_.size()); ++i) {
            if (counts_[i] <= 0) {
                continue;
            }
            out << nominals_[i] << " - "s << counts_[i] << endl;
        }
    }

private:
    vector<T> nominals_;
    vector<int> counts_;
};

```

Однако в реальных задачах не всегда заранее известен небольшой перечень всех возможных объектов, которые предстоит хранить в контейнере. Например, решая задачу о словах в произведении Чосера, вы не имели перед собой словарь среднеанглийского языка. В подобных случаях функция индексации может использовать самые разные принципы.

Для примера создадим класс автомобильного номера. Он будет содержать функцию индексации, реализованную в методе  `Hash`:

```cpp
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

class VehiclePlate {
public:
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{l0, l1, l2}
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        // чтобы дополнить цифровую часть номера слева нулями
        // до трёх цифр, используем подобные манипуляторы:
        // setfill задаёт символ для заполнения,
        // right задаёт выравнивание по правому краю,
        // setw задаёт минимальное желаемое количество знаков
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

```

Такая функция индексации называется  **хеш-функцией**. В коде выше — крайне простая хеш-функция: она возвращает цифровую часть номера, полностью игнорируя буквы и регион, и присваивает номер объекту произвольного типа. Значение хеш-функции объекта называется его  **хешем**.

Хеш-функция позволила бы преодолеть ограничение в задаче прошлого урока. Там для хранения в контейнере требовался список всех возможных элементов. Если для составного объекта (автомобильный номер) в качестве ключа использовать простой объект (число), то можно хранить объекты в векторе, эффективно вычисляя их позицию.

Реализуем контейнер для объектов, поддерживающих индексацию. В этот раз будем определять не количество одинаковых объектов, а их наличие или отсутствие. Для этого хорошо подойдёт тип  `optional`, позволяющий сохранить объект, когда он появится. Элемент вектора можно представлять в виде пустой корзинки, в которую в некоторый момент кладут регистрационный номер автомобиля.

```cpp
#include <optional>
#include <vector>
#include <array>

...

template <typename T>
class HashableContainer {
public:
    void Insert(T elem) {
        int index = elem.Hash();

        // если вектор недостаточно велик для этого индекса,
        // то увеличим его, выделив место с запасом
        if (index >= int(elements_.size())) {
            elements_.resize(index * 2 + 1);
        }

        elements_[index] = move(elem);
    }

    void PrintAll(ostream& out) const {
        for (auto& e : elements_) {
            if (!e.has_value()) {
                continue;
            }
            out << e.value() << endl;
        }
    }

private:
    vector<optional<T>> elements_;
};

```

Как видно, размер вектора изначально не задан, он будет автоматически подстраиваться под помещаемые объекты.

----------

Изучите код контейнера выше и определите, какое максимальное количество корзинок может создаваться при использовании контейнера с автомобильным номером. Учитывайте, что цифровая часть номера — это число от 0 до 999.

Заполнено будет не более 1000 корзинок. Получается, размер вектора максимум в два раза превышает количество номеров. Вполне допустимые издержки. Но у этой программы есть более серьёзный недостаток. Найти его можно, протестировав контейнер и номерной знак:

```cpp
...

int main() {
    HashableContainer<VehiclePlate> plate_base;
    plate_base.Insert({'B','H', 840, 'E', 99});
    plate_base.Insert({'O','K', 942, 'K', 78});
    plate_base.Insert({'H','E', 968, 'C', 79});
    plate_base.Insert({'T','A', 326, 'X', 83});
    plate_base.Insert({'H','H', 831, 'P', 116});
    plate_base.Insert({'P','M', 884, 'K', 23});
    plate_base.Insert({'O','C', 34, 'P', 24});
    plate_base.Insert({'M','Y', 831, 'M', 43});
    plate_base.Insert({'K','T', 478, 'P', 49});
    plate_base.Insert({'X','P', 850, 'A', 50});

    plate_base.PrintAll(cout);
}

```

Запускаем и проверяем ответ:

```cpp
OC034P24
TA326X83
KT478P49
MY831M43
BH840E99
XP850A50
PM884K23
OK942K78
HE968C79

```

Один из номеров пропал.

----------

Куда пропал номер НН831Р116?

-   Он не был выведен из-за того, что код региона не вписался в две цифры.
    
-   Ошибка в функции вывода — она выводит не все элементы вектора.
    
-   Цифровая часть совпала с другим номером, и он перезаписал его.
    

----------

Сколько различных автомобильных номеров может быть представлено объектом класса  `VehiclePlate`? Считайте, что в качестве буквы подставляется один из 12 символов, в качестве кода региона — одно из 122 чисел, а в качестве цифровой части — любое число от 0 до 999.

-   186 412
    
-   2 186 000
    
-   210 816 000
    
-   102 806 210 000

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Изучите код контейнера выше и определите, какое максимальное количество корзинок может создаваться при использовании контейнера с автомобильным номером. Учитывайте, что цифровая часть номера — это число от 0 до 999.

-   Если в контейнер сразу положить номер 999, размер вектора станет 1999 и больше не изменится. Однако если вначале положить 500, то за 1001 размер не выйдет, какие бы номера ни клали впоследствии.

----------

Куда пропал номер НН831Р116?

-   **(-)**  Он не был выведен из-за того, что код региона не вписался в две цифры.
    
-   **(-)**  Ошибка в функции вывода — она выводит не все элементы вектора.
    
-   **(+)**  Цифровая часть совпала с другим номером, и он перезаписал его.
    

----------

Сколько различных автомобильных номеров может быть представлено объектом класса  `VehiclePlate`? Считайте, что в качестве буквы подставляется один из 12 символов, в качестве кода региона — одно из 122 чисел, а в качестве цифровой части — любое число от 0 до 999.

-   **(-)**  186 412
    
-   **(-)**  2 186 000
    
-   **(+)**  210 816 000
    
-   **(-)**  102 806 210 000

</details>

<details>
<summary>Задание:</summary>

## Задание

Сделайте корзины более вместительными, чтобы в них можно было вставить несколько объектов и корректно обработать случай коллизий. Вместо  `optional`  используйте любой удобный контейнер.

Повторные номера хранить не нужно. Чтобы избегать дубликатов, определите  `operator==`  для класса  `VehiclePlate`.

Контейнер должен корректно работать не только для автомобильных номеров, но и для любых объектов, имеющих метод  `Hash`  и  `operator==`.

Учтите, что количество номеров в одной корзинке будет в любом случае невелико, а вот самих корзинок достаточно много. Лучше использовать для хранения данных корзинки простой контейнер, например  `vector`.

При печати номера́ выдавайте в соответствии с номерами корзинок, как в заготовке кода. Номера, попавшие в одну корзинку, выдайте в порядке их поступления в контейнер.

Обратите внимание: буквы в номерах латинские.

### Ограничения

Единственное изменение, которое допускается для класса номера — добавление  `operator==`.

Класс  `HashableContainer`  должен содержать только одно поля типа  `vector<S<T>>`, где  `S`  — некоторый контейнер, выбранный на ваше усмотрение.

### Примеры

```cpp
int main() {
    HashableContainer<VehiclePlate> plate_base;
    plate_base.Insert({'B','H', 840, 'E', 99});
    plate_base.Insert({'O','K', 942, 'K', 78});
    plate_base.Insert({'O','K', 942, 'K', 78});
    plate_base.Insert({'O','K', 942, 'K', 78});
    plate_base.Insert({'O','K', 942, 'K', 78});
    plate_base.Insert({'H','E', 968, 'C', 79});
    plate_base.Insert({'T','A', 326, 'X', 83});
    plate_base.Insert({'H','H', 831, 'P', 116});
    plate_base.Insert({'A','P', 831, 'Y', 99});
    plate_base.Insert({'P','M', 884, 'K', 23});
    plate_base.Insert({'O','C', 34, 'P', 24});
    plate_base.Insert({'M','Y', 831, 'M', 43});
    plate_base.Insert({'B','P', 831, 'M', 79});
    plate_base.Insert({'K','T', 478, 'P', 49});
    plate_base.Insert({'X','P', 850, 'A', 50});

    plate_base.PrintAll(cout);
}

```

Пример вывода:

```cpp
OC034P24
TA326X83
KT478P49
HH831P116
AP831Y99
MY831M43
BP831M79
BH840E99
XP850A50
PM884K23
OK942K78
HE968C79

```

### Как будет тестироваться ваш код

Будет проверено, что:

-   контейнер выводит все номера, которые были в него добавлены,
-   контейнер не выводит номера повторно,
-   контейнер выводит номера в правильном порядке,
-   контейнер корректно работает для объектов другого типа,
-   равные номера определяются правильно,
-   вы не добавляли в класс контейнера новых полей,
-   вы не меняли класс номера.

### Подсказка

-   Используйте в классе  `HashableContainer`  одно поле типа  `vector<vector<T>>`.
-   `operator==`  для класса  `VehiclePlate`  должен проверять равенство всех компонентов номера.
-   При добавлении номера примените алгоритм  `find`, чтобы выяснить, дублируется ли он.

</details>

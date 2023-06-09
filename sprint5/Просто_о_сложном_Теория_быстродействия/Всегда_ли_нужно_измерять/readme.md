<details>
<summary>Введение:</summary>

# Введение

Чтобы решить задачу наиболее эффективным способом, нужно оценить количество операций и сложность алгоритма. Вы научитесь этому в данной теме и поймёте, почему стоит думать о худшем. Худшем случае.

</details>

<details>
<summary>Теория + тестирование:</summary>

# Всегда ли нужно измерять

В прошлой теме вы сделали невероятное: в тысячи раз ускорили операцию реверсирования вектора, просто изменив место вставки элементов. Для достижения конечного результата нет разницы, куда добавлять элемент: в начало или в конец, а с точки зрения влияния на производительность разница огромная. Но, возможно, есть способ и без профилировщика понять, что вставлять нужно именно в конец. Тогда удастся сразу реализовать быстрый вариант.

Иногда действительно можно заранее оценить производительность алгоритма и не тратить время на написание заведомо неприемлемой по скорости реализации. Для примера посчитаем количество элементарных операций для двух способов реверсирования вектора:

**Первый способ:**  вставка элементов происходит в конец вектора.

**Второй способ:**  вставка элементов происходит в начало вектора.

Будем считать только операции определённого вида — запись одного числа в память компьютера. Также будем считать, что мы заранее зарезервировали место в векторе методом  `reserve`, так что вставки в конец требуют только одной операции.

Вектор хранит свои элементы непрерывным блоком. Чтобы вставить элемент в какую-либо позицию, нужно предварительно освободить для него ячейку. Для этого придётся передвинуть все последующие элементы вправо. Таким образом, вставка в начало вектора размера K потребует K + 1 операцию, так как нужно переместить K существующих элементов вектора и записать ещё одно число.

![](https://raw.githubusercontent.com/AYglazk0v/practicum_Cpp_developer/1a7ff48cef0685d458944b84d38935f30f0622e8/sprint5/%D0%9F%D1%80%D0%BE%D1%81%D1%82%D0%BE_%D0%BE_%D1%81%D0%BB%D0%BE%D0%B6%D0%BD%D0%BE%D0%BC_%D0%A2%D0%B5%D0%BE%D1%80%D0%B8%D1%8F_%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B4%D0%B5%D0%B9%D1%81%D1%82%D0%B2%D0%B8%D1%8F/%D0%92%D1%81%D0%B5%D0%B3%D0%B4%D0%B0_%D0%BB%D0%B8_%D0%BD%D1%83%D0%B6%D0%BD%D0%BE_%D0%B8%D0%B7%D0%BC%D0%B5%D1%80%D1%8F%D1%82%D1%8C/img/1.svg)

_Вставка в конец и середину вектора. При вставке в середину пришлось переместить элементы 15, 16 и 23._

Мы посчитали одно перемещение за одну операцию, потому что переместить — это прочитать и записать в другое место, а чтения в данном примере не считаются. Вставка в конец требует только одну операцию записи.

Теперь представим, что вставляем не один раз, а много — обозначим это количество вставок через N.

**Первый способ:**  делаем N раз по одной операции. В сумме получим N.

**Второй способ:**  в первый раз вектор пуст, будет одна операция записи. Во второй раз уже две операции, в третий — три. Просуммируем все количества операций для каждой из N вставок и получим сложное выражение: 1 + 2 + \cdots + N. Можно вычислить эту сумму по формуле суммы арифметической прогрессии. Ответом будет \cfrac{N^2+N}2.

Теперь предположим, что размер вектора 10 000 элементов. Вычислим общее количество операций.

**Первый способ:**  10 000 операций.

**Второй способ:**  50 005 000 операций.

Разница очевидна! Запуская программу, вы оценили это на себе, а вернее — на оборудовании своего компьютера. Причина такого серьёзного отличия в степени 2. Она есть в формуле второго способа, а в формуле первого её нет.

Наглядно покажет разницу N и \cfrac{N^2 + N}2 такой график:

![2.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint5/%D0%9F%D1%80%D0%BE%D1%81%D1%82%D0%BE_%D0%BE_%D1%81%D0%BB%D0%BE%D0%B6%D0%BD%D0%BE%D0%BC_%D0%A2%D0%B5%D0%BE%D1%80%D0%B8%D1%8F_%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B4%D0%B5%D0%B9%D1%81%D1%82%D0%B2%D0%B8%D1%8F/%D0%92%D1%81%D0%B5%D0%B3%D0%B4%D0%B0_%D0%BB%D0%B8_%D0%BD%D1%83%D0%B6%D0%BD%D0%BE_%D0%B8%D0%B7%D0%BC%D0%B5%D1%80%D1%8F%D1%82%D1%8C/img/2.png?raw=true)


_Сравнение величин при N до 10_

![3.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint5/%D0%9F%D1%80%D0%BE%D1%81%D1%82%D0%BE_%D0%BE_%D1%81%D0%BB%D0%BE%D0%B6%D0%BD%D0%BE%D0%BC_%D0%A2%D0%B5%D0%BE%D1%80%D0%B8%D1%8F_%D0%B1%D1%8B%D1%81%D1%82%D1%80%D0%BE%D0%B4%D0%B5%D0%B9%D1%81%D1%82%D0%B2%D0%B8%D1%8F/%D0%92%D1%81%D0%B5%D0%B3%D0%B4%D0%B0_%D0%BB%D0%B8_%D0%BD%D1%83%D0%B6%D0%BD%D0%BE_%D0%B8%D0%B7%D0%BC%D0%B5%D1%80%D1%8F%D1%82%D1%8C/img/3.png?raw=true)

_Сравнение величин при N до 100_

Алгоритм, в котором встречается N^2, возникает и в обычных жизненных ситуациях.

----------

Представьте большой офис, где работают 50 сотрудников. Приходя, они здороваются с каждым за руку.

Как вы думаете, сколько рукопожатий будет сделано после того, как вошёл последний? Можете посчитать сами или написать для этого небольшую программу.

-   50
    
-   100
    
-   1000
    
-   1225
    
-   2450
    
-   2500
    

Найдём ответ в общем виде для офиса, где N сотрудников. Каждый из N человек пожал руку остальным N-1. Получилось N\cdot(N-1) рукопожатий. Но в рассуждении есть ошибка: жмут руку два человека, поэтому каждое рукопожатие посчитано два раза — в количестве рукопожатий первого и второго. Значит, результат нужно поделить на два. Получится ответ, похожий на то, что мы уже видели: \cfrac{N^2 - N}2. Отличные условия для вирусных инфекций!

----------

Теперь посчитаем, сколько раз за весь день открывали и закрывали дверь, если каждый сотрудник делает это четыре раза:

1.  когда приходит на работу,
2.  когда уходит,
3.  когда идёт на обед,
4.  когда возвращается с обеда.

А ещё в этот день один раз приходила уборщица и два раза курьер.

----------

Как вы думаете, что происходит чаще: открывание двери или рукопожатия? Посчитайте, сколько раз двери пришлось открыться и сколько раз закрыться за один день.

-   53
    
-   106
    
-   206
    
-   1012
    

На этот раз подсчёт проще. Достаточно умножить количество сотрудников на количество открываний двери и прибавить остальных. Общая формула выглядит так: 4N+6. Никакого квадрата тут нет, и количество получилось куда скромнее.

----------

Посмотрите листинг — в нём приведено решение задачи, которую вы видели в одном из прошлых уроков. Чтобы программа была самодостаточной, стек заменён на  `std::stack`:

```cpp
#include <stack>

using namespace std;

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        if (elements_.empty()) {
            elements_.push(element);
            return;
        }

        Type last_elem = elements_.top();
        if (last_elem < element) {
            elements_.pop();
            Push(element);

            elements_.push(last_elem);
        } else {
            elements_.push(element);
        }
    }

    // остальные функции не понадобятся
private:
    stack<Type> elements_;
};

```

Посчитайте:

-   количество вставок в стек (имеется в виду внутренний стек —  `elements_`),
-   количество сравнений (сколько раз будет проверяться условие второго  `if`),

если добавляются числа 1, 2, 3, … 100.

Если посчитать математически не получается, напишите небольшую программу для подсчёта. Оба ответа запишите подряд одним числом, без пробела, в текстовое поле:

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

----------

Представьте большой офис, где работают 50 сотрудников. Приходя, они здороваются с каждым за руку.

Как вы думаете, сколько рукопожатий будет сделано после того, как вошёл последний? Можете посчитать сами или написать для этого небольшую программу.

-   **(-)**  50
    
-   **(-)**  100
    
-   **(-)**  1000
    
-   **(+)**  1225
    
-   **(-)**  2450
    
-   **(-)**  2500
    

----------

Как вы думаете, что происходит чаще: открывание двери или рукопожатия? Посчитайте, сколько раз двери пришлось открыться и сколько раз закрыться за один день.

-   **(-)**  53
    
-   **(-)**  106
    
-   **(+)**  206
    
-   **(-)**  1012
    

----------

Посмотрите листинг — в нём приведено решение задачи, которую вы видели в одном из прошлых уроков. Чтобы программа была самодостаточной, стек заменён на  `std::stack`:

```cpp
#include <stack>

using namespace std;

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        if (elements_.empty()) {
            elements_.push(element);
            return;
        }

        Type last_elem = elements_.top();
        if (last_elem < element) {
            elements_.pop();
            Push(element);

            elements_.push(last_elem);
        } else {
            elements_.push(element);
        }
    }

    // остальные функции не понадобятся
private:
    stack<Type> elements_;
};

```

Посчитайте:

-   количество вставок в стек (имеется в виду внутренний стек —  `elements_`),
-   количество сравнений (сколько раз будет проверяться условие второго  `if`),

если добавляются числа 1, 2, 3, … 100.

Если посчитать математически не получается, напишите небольшую программу для подсчёта. Оба ответа запишите подряд одним числом, без пробела, в текстовое поле:

-   Правильный ответ: 50504950

</details>

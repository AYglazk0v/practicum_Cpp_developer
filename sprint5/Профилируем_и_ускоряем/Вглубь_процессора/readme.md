<details>
<summary>Теория + тестирование:</summary>

# Вглубь процессора

Оптимизация — хорошая вещь, позволяющая заменить операции программы на более эффективные и тем самым ускорить вычисление на несколько процентов или в сотни раз. Разберёмся, можно ли оптимизировать программу, не меняя операции, которые она совершает.

Представим, что в некоторой обсерватории есть телескоп, за одну ночь совершающий измерения на  `n`  участках неба. Результат измерения кодируется одним латинским символом от A до Z. В результате получается строка длины  `n`.

Собраны результаты за длительный период работы телескопа. Среди них нужно найти участки неба, где больше всего сильных сигналов — с литерой K или далее. Реализуем функцию подсчёта сильных сигналов по каждому участку неба:

```cpp
#include <string>
#include <vector>

using namespace std;

// Функция анализирует данные телескопа, определяя, сколько сильных сигналов
// зафиксировано по каждому направлению.
vector<int> ComputeStatistics(const vector<string>& measures, int n) {
    int m = measures.size();
    vector<int> result(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (measures[j][i] >= 'K') {
                ++result[i];
            }
        }
    }

    return result;
}

```

Функция возвращает вектор, элементы которого — это количества сильных сигналов на нулевом участке неба, первом, втором. Протестируем эту функцию, измерив время. Считаем, что прошло 5000 дней и количество участков неба тоже равно 5000. Реальных данных телескопа у нас под рукой нет, поэтому заполним вектор случайными буквами:

```cpp
#include "log_duration.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

// функция анализирует данные телескопа, определяя, сколько сильных сигналов
// зафиксировано по каждому направлению
vector<int> ComputeStatistics(const vector<string>& measures, int n) {
    int m = measures.size();
    vector<int> result(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // прибавляем 1, если сигнал не менее 'K'
            result[i] += (measures[j][i] >= 'K' ? 1 : 0);
        }
    }

    return result;
}

string GetRandomString(int size) {
    static mt19937 engine;
    uniform_int_distribution<int> distribution('A', 'Z');

    string res(size, ' ');
    for (char& c : res) {
        c = char(distribution(engine));
    }

    return res;
}

int main() {
    vector<string> data;

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomString(5000));
    }

    vector<int> statistics;
    {
        LOG_DURATION("ComputeStatistics"s);
        statistics = ComputeStatistics(data, 5000);
    }

    cout << "Всего сильных сигналов: "s << accumulate(statistics.begin(), statistics.end(), 0) << endl;
}

```

Результат может быть таким:

```cpp
ComputeStatistics: 134 ms
Всего сильных сигналов: 15379777

```

Подумаем, много это или мало — 134 миллисекунды. За такое время процедура обрабатывает 5000 × 5000, то есть 25 миллионов байт. Получается, что за секунду было бы обработано около 187 мегабайт. Сравним с другой программой, например, видеоплеером. Современные видеоплееры должны декодировать и показать в секунду минимум 25 кадров в разрешении 4K, то есть 4096 × 3072 × 25 байт. Это примерно 315 мегабайт. Что-то подсказывает, что декодирование — куда более сложная операция, чем простой подсчёт количества определённых букв. Значит, в нашей программе что-то не так.

Внесём в программу минимальные изменения — поменяем порядок циклов в основной процедуре подсчёта:

```cpp
vector<int> ComputeStatistics2(const vector<string>& measures, int n) {
    int m = measures.size();
    vector<int> result(n);

    // теперь первым идёт цикл по j
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            // прибавляем 1, если сигнал не менее 'K'
            result[i] += (measures[j][i] >= 'K' ? 1 : 0);
        }
    }

    return result;
}

```

----------

Что поменялось при выполнении программы?

-   Стало больше сложений.
    
-   Стало меньше сравнений и проверок:  `x < y`.
    
-   Теперь другой порядок чтения данных и сложения.
    
-   Ничего, будет выполняться в точности так же, как раньше.
    

Запустим эту программу и сравним две функции:

```cpp
ComputeStatistics: 130 ms
ComputeStatistics2: 42 ms
Всего сильных сигналов: 15379777

```

Неожиданный результат: выигрыш в три раза. Наверное, мы что-нибудь перепутали, ведь процессору всё равно, в каком порядке выполнять сложения, если их количество не изменилось.

При оптимизации и другом рефакторинге нужно контролировать, что ответ оптимизированной функции будет таким же, как ответ старой, проверенной. Сделаем это. Если ответы совпадут, значит,  `ComputeStatistics2`  выполняет свою задачу, причём делает это в разы быстрее. Вставим в  `main`  такой код:

```cpp
...
vector<int> statistics2;
{
    LOG_DURATION("ComputeStatistics2"s);
    statistics2 = ComputeStatistics2(data, 5000);
}

cout << (statistics == statistics2 ? "OK"s : "Fail"s) << endl;
...

```

Запустим ещё раз:

```cpp
ComputeStatistics: 131 ms
ComputeStatistics2: 48 ms
OK
Всего сильных сигналов: 15379777

```

Ответы совпали! Чтобы понять, почему один алгоритм быстрее другого, нужно углубиться в то, как работают RAM и кэш процессора — специальная быстрая память, с которой процессор оперирует напрямую.

Данные из RAM помещаются в кэш не по одному байту, а блоками. Поэтому когда из объекта  `string`  читается один символ, все соседние тоже загружаются в кэш и доступ к ним происходит быстрее, чем к далёким символам и элементам других строк.

Фрагмент кэша, хранящий один блок, называется «кэш-линия». Процессоры имеют несколько кэш-линий. Их размер зависит от архитектуры процессора. У многих современных процессоров он равен 64 байтам. Каждая кэш-линия хранит участок значений, к которым недавно обращалась какая-либо работающая программа.

Если вы сразу обработаете все значения, попавшие в одну кэш-линию, процессору не придётся перезаписывать кэш и обращаться к медленной памяти RAM каждый раз при чтении символа из строки.

Символы разных строк располагаются в памяти далеко и, скорее всего, не попадут в кэш-линию одновременно. Поэтому если программа попеременно обращается к разным строкам, процессор будет постоянно обновлять кэш-линии и простаивать, ожидая, пока данные из RAM попадут в кэш.

Когда внутренний цикл проходится по одной строке, её большой кусок загружается в кэш. Происходит работа над данными одной кэш-линии до тех пор, пока все они не будут обработаны. Если же внутренний цикл проходит «по вертикали», то из кэш-линии вы по сути обрабатываете только один символ.

Вы могли заметить, что прибавление единицы было странно реализовано через тернарный оператор. При этом явно происходят прибавления нуля — лишние сложения, которые ничего не меняют. Попробуем ускорить программу ещё, убрав их:

```cpp
vector<int> ComputeStatistics3(const vector<string>& measures, int n) {
    int m = measures.size();
    vector<int> result(n);

    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            // будем прибавлять только когда надо
            if (measures[j][i] >= 'K') {
                ++result[i];
            }
        }
    }

    return result;
}

```

Надеемся на большее ускорение и запускаем:

```cpp
ComputeStatistics: 142 ms
ComputeStatistics2: 41 ms
ComputeStatistics3: 198 ms
OK
Всего сильных сигналов: 15379777

```

Вот это дела! Мы уменьшили количество сложений, не добавив, казалось бы, новых операций, а скорость при этом упала почти в пять раз.

Причина такого поведения программы тоже в особенностях процессоров.  `if`  вставляет специальную процессорную инструкцию — условный переход. Эта инструкция — одна из самых сложных и неприятных для процессора. Он не может работать эффективно, когда не знает, что ему делать дальше. При условном переходе это неизвестно, поэтому процессор как бы впадает в ступор. В критических местах программы нужно избегать  `if`, предпочитая тернарный оператор или другие средства. Но тернарный оператор и цикл тоже могут создавать инструкцию условного перехода. Они будут избегать её лишь в определённых простых случаях.

Выходит, самая эффективная реализация — вторая. В ней память обрабатывается последовательно, отсутствует  `if`.

Мы проделали неплохую работу, сравнив разные реализации. Измерения действительно были полезны. Особенно когда мы были уверены, что улучшим производительность, убрав ненужные прибавления, а вышло наоборот. Этот пример показывает, что предусмотреть все нюансы нельзя, нужно измерять.

Реализованная процедура  `ComputeStatistics2`  уже достаточно эффективна. Но можно применить ещё несколько оптимизаций, выходящих за рамки этого урока:

-   Специальные функции, которые позволяют производить одну и ту же операцию над несколькими числами одновременно, — в нашем случае сравнение и сложение. Эта технология называется Single Instruction Multiple Data или SIMD. О таких функциях можно почитать в  [гайде от Intel](https://software.intel.com/sites/landingpage/IntrinsicsGuide/). Когда сайт недоступен или не работает, используйте его  [зеркало](https://www.laruence.com/sse/#).
-   Разворачивание цикла. Дело в том, что  `for`  тоже содержит условный переход в начале каждой итерации. Для оптимизации можно обработать в одной итерации сразу несколько элементов — например 16. Так мы сократим количество итераций и, соответственно, условных переходов в 16 раз. Это достаточно спорный подход, так как оптимизаторы иногда делают разворачивание цикла автоматически, а понятность кода при этом ухудшается.
-   Распараллеливание. Можно поручить нескольким потокам выполнения обрабатывать разные участки вектора. Это должно существенно ускорить работу на многоядерных компьютерах, если другие ядра не задействованы. Когда компьютер одновременно считает статистику с нескольких телескопов, распараллеливание вряд ли даст выигрыш. О многопоточных программах вы узнаете подробно в последующих спринтах.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания


Что поменялось при выполнении программы?

-   **(-)**  Стало больше сложений.
    
-   **(-)**  Стало меньше сравнений и проверок:  `x < y`.
    
-   **(+)**  Теперь другой порядок чтения данных и сложения.
    
-   **(-)**  Ничего, будет выполняться в точности так же, как раньше.

</details>


<details>
<summary>Задание:</summary>

### Задание

Метеостанция измеряет температуру определённое количество раз в течение дня. Каждый день измерения производятся в одно и то же время, и их количество одинаково. Данные, полученные в течение одного дня, сохраняются в виде вектора чисел  `float`. В результате наблюдений в течение нескольких дней получается набор векторов типа  `vector<float>`. Вычислите среднее значение измерений, сделанных в одно и то же время, но в разные дни. При этом учитывайте только положительные температуры.

Например, если наблюдения велись 30 дней, каждый день делалось по 1440 измерений, то на вход поступит 30 векторов по 1440 значений. Программа должна построить новый вектор из 1440 значений типа  `float`, каждое из которых — среднее значение положительных температур за соответствующую минуту.

Реализуйте функцию  `ComputeAvgTemp`. Она должна возвращать вектор, который содержит среднюю положительную температуру для каждого измерения. Если для какого-либо измерения все температуры неположительные, запишите для этого измерения ноль.

Количество измерений в день определите сами по размеру первого вектора. Для пустого набора верните пустой ответ. Гарантируется, что все векторы измерений имеют одинаковый размер.

Ваша реализация должна быть эффективна: правильно расположите вложенные циклы, избегайте  `if`. Учтите, что деление чисел — тоже долгая операция, следует минимизировать количество делений.

### Формат выходных данных

Функция ничего не должна выводить в потоки.

### Ограничения

Ваша реализация должна быть эффективной по сложности и использованию кэша. Она не должна делать избыточных условных переходов и делений.

### Что отправлять на проверку

Загрузите в тренажёр код, содержащий функцию  `ComputeAvgTemp`. Функция  `main`  учитываться не будет.

### Как будет тестироваться ваш код

Будет проверено, что функция  `ComputeAvgTemp`  работает правильно и эффективно.

### Пример

```cpp
void Test() {
    // 4 дня по 3 измерения
    vector<vector<float>> v = {
        {0, -1, -1},
        {1, -2, -2},
        {2, 3, -3},
        {3, 4, -4}
    };

    // среднее для 0-го измерения (1+2+3) / 3 = 2 (не учитывам 0)
    // среднее для 1-го измерения (3+4) / 2 = 3.5 (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    assert(ComputeAvgTemp(v) == vector<float>({2, 3.5f, 0}));
}

```

### Подсказка

-   Чтобы создать литерал типа  `float`, напишите после числа с точкой букву  `f`. Либо целую, либо дробную часть можно опускать, если подразумевается ноль. Примеры литералов:  `3.14f`,  `-.2f`,  `20.f`.
-   Напишите два вложенных цикла. Внешний будет пробегать по векторам, а внутренний — по измерениям. Внутри циклов используйте тернарные операторы, но не  `if`.
-   Заведите два вектора — один для суммы положительных температур по каждому измерению, второй для их количества. Также вы можете использовать один вектор структур, содержащих два поля.
-   Таким образом, вы просуммируете все положительные измерения и получите их количество. Среднее — это отношение суммы к количеству. Выполните деление в конце, после того как посчитаете все суммы целиком.
-   Будьте внимательны: делить следует на количество положительных слагаемых, а не на количество всех дней.

</details>

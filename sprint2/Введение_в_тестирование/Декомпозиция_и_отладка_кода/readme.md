<details>  
<summary>Теория:</summary>

# Декомпозиция и отладка кода

Вы научились декомпозировать и тестировать код. В этом уроке вы примените новые навыки для проверки задачи «‎Автобусные остановки». Ниже даны условие и правильное решение, целиком содержащееся внутри функции  `main`. Сделайте декомпозицию решения на заранее заданные блоки так, чтобы получившаяся программа корректно решала задачу.

### Условие задачи

Реализуйте систему хранения автобусных маршрутов. Обработайте следующие запросы:

-   **NEW_BUS bus stop_count stop1 stop2**  … — добавить маршрут автобуса с параметрами  `bus`,  `stop_count`  и  `stop1`,  `stop2`, … Эти параметры команды  **NEW_BUS**  задают название маршрута (`bus`), количество остановок (`stop_count`) и их названия. Например:  `NEW_BUS golden_ring 4 sergiev_posad rostov ivanovo vladimir`  задаёт маршрут с названием  `golden_ring`  и четырьмя остановками:  `sergiev_posad`,  `rostov`,  `ivanovo`,  `vladimir`.
-   **BUSES_FOR_STOP stop**  — вывести названия всех маршрутов автобуса, проходящих через остановку  `stop`.
-   **STOPS_FOR_BUS bus**  — вывести названия всех остановок маршрута  `bus`  со списком автобусов, куда можно пересесть на каждой из остановок.
-   **ALL_BUSES**  — вывести список всех маршрутов с остановками.

### Формат ввода

В первой строке ввода содержится количество запросов Q, затем в Q строках следуют описания запросов.

Гарантируется, что все названия маршрутов и остановок состоят лишь из латинских букв, цифр и знаков подчёркивания.

Для каждого запроса  **NEW_BUS bus stop_count stop1 stop2 …**  гарантируется, что маршрут  `bus`  отсутствует, количество остановок больше нуля, а после числа  `stop_count`  следует именно такое количество названий остановок, причём все названия в каждом списке различны.

### Формат вывода

Для каждого запроса кроме  **NEW_BUS**  выведите соответствующий ответ на него:

-   На запрос  **BUSES_FOR_STOP stop**  выведите через пробел список автобусов, проезжающих через эту остановку, в том порядке, в котором они создавались командами  **NEW_BUS**. Если остановка  `stop`  не существует, выведите  `No stop`.
-   На запрос  **STOPS_FOR_BUS bus**  выведите описания остановок маршрута  `bus`  в отдельных строках в том порядке, в котором они были заданы в соответствующей команде  **NEW_BUS**. Описание каждой остановки  `stop`  должно иметь вид  **Stop stop: bus1 bus2 …**, где  **bus1 bus2 …**  — список автобусов, проезжающих через остановку  `stop`. Список должен быть в том порядке, в каком автобусы создавались командами  **NEW_BUS**, за исключением исходного маршрута  `bus`. Если через остановку  `stop`  не проезжает ни один автобус кроме  `bus`, вместо списка автобусов для неё выведите “no interchange**”**. Если маршрут  `bus`  не существует, выведите  `No bus`.
-   На запрос  **ALL_BUSES**  выведите описания всех автобусов в алфавитном порядке. Описание каждого маршрута  `bus`  должно иметь вид  **Bus bus: stop1 stop2**  …, где  **stop1 stop2 …**  — список остановок автобуса  `bus`  в том порядке, в каком они были заданы в соответствующей команде  **NEW_BUS**. Если автобусы отсутствуют, выведите  `No buses`.

### Предупреждение

В условии задачи много важных деталей. Чтобы не упустить ни одной, перечитайте его.

### Примеры

**Ввод**

```
10
ALL_BUSES
BUSES_FOR_STOP Marushkino
STOPS_FOR_BUS 32K
NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo
NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
BUSES_FOR_STOP Vnukovo
NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo
STOPS_FOR_BUS 272
ALL_BUSES

```

**Вывод**

```
No buses
No stop
No bus
32 32K
Stop Vnukovo: 32 32K 950
Stop Moskovsky: no interchange
Stop Rumyantsevo: no interchange
Stop Troparyovo: 950
Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
Bus 32: Tolstopaltsevo Marushkino Vnukovo
Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo

```

## Исходные файлы

Решение этой задачи:

```cpp
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    int q;
    cin >> q;

    map<string, vector<string>> buses_to_stops, stops_to_buses;

    for (int i = 0; i < q; ++i) {
        string operation_code;
        cin >> operation_code;

        if (operation_code == "NEW_BUS"s) {
            string bus;
            cin >> bus;
            int stop_count;
            cin >> stop_count;
            vector<string>& stops = buses_to_stops[bus];
            stops.resize(stop_count);
            for (string& stop : stops) {
                cin >> stop;
                stops_to_buses[stop].push_back(bus);
            }

        } else if (operation_code == "BUSES_FOR_STOP"s) {
            string stop;
            cin >> stop;
            if (stops_to_buses.count(stop) == 0) {
                cout << "No stop"s << endl;
            } else {
                for (const string& bus : stops_to_buses[stop]) {
                    cout << bus << " "s;
                }
                cout << endl;
            }

        } else if (operation_code == "STOPS_FOR_BUS"s) {
            string bus;
            cin >> bus;
            if (buses_to_stops.count(bus) == 0) {
                cout << "No bus"s << endl;
            } else {
                for (const string& stop : buses_to_stops[bus]) {
                    cout << "Stop "s << stop << ": "s;
                    if (stops_to_buses[stop].size() == 1) {
                        cout << "no interchange"s;
                    } else {
                        for (const string& other_bus : stops_to_buses[stop]) {
                            if (bus != other_bus) {
                                cout << other_bus << " "s;
                            }
                        }
                    }
                    cout << endl;
                }
            }

        } else if (operation_code == "ALL_BUSES"s) {
            if (buses_to_stops.empty()) {
                cout << "No buses"s << endl;
            } else {
                for (const auto& bus_item : buses_to_stops) {
                    cout << "Bus "s << bus_item.first << ": "s;
                    for (const string& stop : bus_item.second) {
                        cout << stop << " "s;
                    }
                    cout << endl;
                }
            }
        }
    }

    return 0;
}
```

</details>  

<details>  
<summary>Задание:</summary>

## Задание

Перед вами шаблон с заготовками классов и функций. Не меняя его функцию  `main`, реализуйте эти классы и функции так, чтобы получившаяся программа решала задачу «‎Автобусные остановки».

### Как будет тестироваться ваша программа

Автоматическая проверяющая система заменит в вашей программе функцию  `main`  на ту, которая дана в шаблоне, скомпилирует получившийся файл и прогонит на тестах для задачи «‎Автобусные остановки».

### Рекомендации

Выполняя декомпозицию программы, пишите модульные тесты на получающиеся классы и функции. Для модульного тестирования операций воспользуйтесь классами  `istringstream`  и  `ostringstream`, как в примере:

```cpp
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

struct Point {
    int x = 0;
    int y = 0;
};

ostream& operator<<(ostream& output, Point p) {
    output << p.x << ' ' << p.y;
    return output;
}

istream& operator>>(istream& input, Point& p) {
    input >> p.x >> p.y;
    return input;
}

void TestPointOutput() {
    Point p = {15, -8};
    ostringstream output;
    output << p;
    // Метод str() возвращает строку с содержимым, выведенным в ostringstream
    assert(output.str() == "15 -8"s);
}

void TestPointInput() {
    istringstream input;
    // Метод str(строка) у istringstream позволяет задать содержимое, которое будет считываться из istringstream
    input.str("-9 33"s);

    Point p;
    input >> p;
    assert(p.x == -9);
    assert(p.y == 33);
}

int main() {
    TestPointOutput();
    TestPointInput();
}

```

### Подсказка

На первом шаге реализуйте оператор чтения структуры  `Query`  и, применив его, обновите исходный код функции  `main`. Затем наполните внутренности структур  `AllBusesResponse`,  `BusesForStopResponse`  и  `AllBusesResponse`  и перенесите логику обработки запросов в  `BusManager`, оставив внутри  `main`  только ввод и вывод. На последнем этапе реализуйте операторы вывода структур, приведя тело функции  `main`  к финальному варианту.

</details>  

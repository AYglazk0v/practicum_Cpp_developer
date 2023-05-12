<details>
<summary>Теория + тестирование:</summary>

# Упрощаем логирование

Выполняя задание в прошлом уроке, вы могли заметить, что это довольно утомительная работа. А главное — легко ошибиться и перепутать многочисленные переменные типа  `time_point`. К счастью, в C++ есть замечательный механизм, который позволяет выполнять парные действия удобно и без риска ошибок.

----------

Как вы думаете, какой механизм упростит измерение времени?

-   Конструктор и деструктор класса как раз подходят для того, чтобы выполнять парные действия.
    
-   Подойдёт обычный  `for`, который сделает две итерации: на первой измерит время начала, на второй — время конца.
    
-   Тут не обойтись без исключений.
    

Можем использовать классы. Сейчас увидите, что конструктор и деструктор отлично решают задачу.

Создадим класс  `LogDuration`, объект которого при создании и уничтожении будет запоминать время. При уничтожении он выведет продолжительность операции в  `cerr`:

```cpp
class LogDuration {
public:
    LogDuration() {
    }

    ~LogDuration() {
        const auto end_time = steady_clock::now();
        const auto dur = end_time - start_time_;
        cerr << duration_cast<milliseconds>(dur).count() << " ms"s << endl;
    }

private:
    // В переменной будет время конструирования объекта LogDuration
    const steady_clock::time_point start_time_ = steady_clock::now();
};

```

`LogDuration`  упростит профилировку. Надо просто создать переменную типа  `LogDuration`  с любым названием, и она автоматически измерит время, прошедшее от своего создания до выхода из области видимости. Рассмотрим работу класса  `LogDuration`  на примере:

```cpp
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
using namespace literals;

class LogDuration {
public:
    LogDuration() {
    }

    ~LogDuration() {
        // эта переменная сохранит время уничтожения объекта:
        const auto end_time = steady_clock::now();
        const auto dur = end_time - start_time_;
        cerr << duration_cast<milliseconds>(dur).count() << " ms"s << endl;
    }

private:
    // Переменная будет инициализирована текущим моментом времени при
    // создании объекта класса.
    // Таким образом, она хранит время создания объекта.
    const steady_clock::time_point start_time_ = steady_clock::now();
};

int main() {
    cout << "Ожидание 5s..."s << endl;

    {
        LogDuration sleep_guard;
        // операция - ожидание 5 секунд
        this_thread::sleep_for(5s);
    }

    cout << "Ожидание завершено"s << endl;
}

```

Запустим программу и убедимся в корректной работе  `LogDuration`  :

```
Ожидание 5s...
5000 ms
Ожидание завершено
```

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Как вы думаете, какой механизм упростит измерение времени?

-   **(+)**  Конструктор и деструктор класса как раз подходят для того, чтобы выполнять парные действия.
    
-   **(-)**  Подойдёт обычный  `for`, который сделает две итерации: на первой измерит время начала, на второй — время конца.
    
-   **(-)**  Тут не обойтись без исключений.

</details>

<details>
<summary>Задание:</summary>

### Задание

Примените класс в действии и упростите профилировку. Но сначала слегка доработайте  `LogDuration`, чтобы он ничем не уступал «ручному» способу.

1.  Расширьте класс  `LogDuration`, добавив в него поле и параметр конструктора типа  `std::string`, чтобы он мог выводить название операции так, как это было в «ручном» примере.
2.  Переделайте логирование в коде из предыдущего урока, используя класс  `LogDuration`. Также измерьте суммарное время работы всей функции  `Operate`. Назовите его “Total”.

Вынесите класс  `LogDuration`  в отдельный файл  `log_duration.h`.

### Ограничения

Не меняйте поведение и вывод программы. Она должна делать ровно то же самое, но выводить измерения времени другим способом.

В  `main`  не измеряйте время старым способом.

### Пример

После доработки можно будет использовать класс так:

```cpp
int main() {
    // название переменной не играет роли
    LogDuration guard("Total sleeping"s);

    {
        LogDuration guard("Sleeping 1 sec"s);
        this_thread::sleep_for(chrono::seconds(1));
    }

    {
        LogDuration guard("Sleeping 2 sec"s);
        this_thread::sleep_for(chrono::seconds(2));
    }
}

```

Возможный вывод этого примера:

```
Sleeping 1 sec: 1001 ms
Sleeping 2 sec: 2003 ms
Total sleeping: 3004 ms

```

### Пример вывода

```
Append random: 5 ms
Reverse: 5811 ms
After 1 bits we found 0% pops
After 2 bits we found 0% pops
After 4 bits we found 25% pops
After 8 bits we found 37.5% pops
After 16 bits we found 50% pops
After 32 bits we found 50% pops
After 64 bits we found 54.6875% pops
After 128 bits we found 53.9063% pops
After 256 bits we found 53.5156% pops
After 512 bits we found 51.7578% pops
After 1024 bits we found 51.9531% pops
After 2048 bits we found 50.4395% pops
After 4096 bits we found 49.585% pops
After 8192 bits we found 49.7681% pops
After 16384 bits we found 49.3469% pops
After 32768 bits we found 49.5178% pops
After 65536 bits we found 49.6353% pops
After 131072 bits we found 50% pops
Counting: 8 ms
Total: 5828 ms

```

### Что отправлять на проверку

Заготовку кода, в которой вы исправили способ измерения времени и усовершенствовали класс  `LogDuration`.

### Как будет тестироваться ваш код

Будет проверено, что:

-   Вы не изменили поведение программы.
-   Класс  `LogDuration`  соответствует требованиям.
-   Вы не измеряете время старым способом — без  `LogDuration`.

### Подсказка

Поместите каждую из трёх операций в фигурные скобки, создав область видимости. В начало скобок добавьте объявление переменной типа  `LogDuration`. Также не забудьте измерить время всей функции  `Operate`. Переменная, объявленная внутри фигурных скобок, никогда не видна вне их. Вынесите объявление переменной  `reversed_bits`  за скобки, а присвоение ей значения оставьте внутри.

</details>

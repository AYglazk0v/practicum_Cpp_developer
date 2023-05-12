
<details>
<summary>Теория:</summary>

# Призываем макросы

В прошлом уроке вы упростили профилировку. Технология, которую вы использовали, называется RAII. Она применяется не только для контроля за ресурсами, но и для планирования действия, которое должно совершиться при выходе из области видимости. Эту технологию вы подробно изучите далее в курсе.

Название переменной, которая позволила создать объект  `LogDuration`, не несёт в себе никакой информации. Более того, мы даже к этой переменной не обращаемся. Чтобы скрыть ненужную информацию, используем препроцессор. С ним вы уже сталкивались во втором спринте. Определим такой макрос:

```cpp
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

```

Далее нужно разработать макрос  `UNIQUE_VAR_NAME_PROFILE`, который будет выдавать уникальное имя переменной. Для профилировщика это не так важно — вряд ли у нас будут две профилировки в одном пространстве имён. Но всё-таки это удобно.

Можем использовать макрос  `__LINE__`. Препроцессор заменяет его на номер строки, в которой использован этот макрос. Нам также понадобится оператор слияния лексем  `##`:

```cpp
#define UNIQUE_VAR_NAME_PROFILE profile_guard_ ## __LINE__

int main() {
    int UNIQUE_VAR_NAME_PROFILE;
}

```

Забавно, в этой программе нет  `#include`, при этом она совершенно корректна, хотя ничего не делает — даже единственную переменную исключит оптимизатор. Посмотрим, что на выходе у препроцессора:

```cpp
#line 1 "main.cpp"

int main() {
    int profile_guard___LINE__;
}

```

Этот файл мы получили, применив специальный режим компилятора. В таком режиме он запускает только препроцессор. Для компилятора GCC этот режим можно запустить из командной строки, указав специальный флаг  `-E`:

```bash
gcc -E main.cpp -o main.i

```

Ожидали мы не этого. Очевидно, препроцессор «приклеил»  `__LINE__`  слишком рано, до того, как подставил номер строки. Чтобы подставить значение  `__LINE__`  до приклеивания к  `profile_guard_`, вынесем слияние лексем в отдельное макроопределение,  `PROFILE_CONCAT`:

```cpp
#define PROFILE_CONCAT(X, Y) X ## Y
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)

int main() {
    int UNIQUE_VAR_NAME_PROFILE;
}

```

Код стал на одну строчку больше. Проверим результат:

```cpp
#line 1 "main.cpp"

int main() {
    int profile_guard___LINE__;
}

```

Результат не изменился. Почти смирившись, что ничего не выйдет, сделаем последнюю попытку. Добавим ещё один макрос:

```cpp
#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)

int main() {
    int UNIQUE_VAR_NAME_PROFILE;
}

```

Посмотрим, что выдал препроцессор:

```cpp
#line 1 "main.cpp"

int main() {
    int profile_guard_6;
}

```

Ничего себе, получилось!

Окончательный вид будет таким:

```cpp
#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
```

</details>

<details>
<summary>Задание:</summary>

### Задание

Замените в примере с профилировкой явное объявление переменных на использование макроса  `LOG_DURATION`.

### Ограничения

Не меняйте поведение программы.

### Что отправлять на проверку

Загрузите в тренажёр решение предыдущей задачи с добавленным макросом  `LOG_DURATION`. Прямое использование  `LogDuration`  нужно заменить на использование этого макроса.

### Как будет тестироваться ваш код

Программа будет проверена теми же тестами, что и предыдущая. Дополнительно будет проверено, что вы не используете  `LogDuration`  напрямую, а только через макрос.

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

### Подсказка

Добавьте макрос в h-файл. В cpp-файле уже не нужно использовать класс  `LogDuration`  напрямую.

</details>

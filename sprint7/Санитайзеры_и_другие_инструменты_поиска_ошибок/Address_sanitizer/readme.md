<details>
<summary>Теория:</summary>

# Address sanitizer

Не все ошибки сводятся к неправильному использованию стандартной библиотеки. И следовательно, не все из них можно обнаружить проверками, которые включаются отладочными макроопределениями.

Современные компиляторы имеют дополнительные инструменты, которые выполняют проверки, не предусмотренные в коде программы. Они называются «санитайзеры». Санитайзеры позволяют собрать программу в режиме усиленного поиска ошибок.

Рассмотрим функцию, которая ищет медианную строку:

```cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string& GetMedianString(vector<string> v) {
    nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
    const string& mid = v[v.size() / 2];

    return mid;
}

int main() {
    vector<string> v = {"cat"s, "dog"s, "elephant"s, "monkey"s, "llama"s};
    const string& res = GetMedianString(v);
    cout << res << endl;
}

```

Скомпилируем и запустим программу:

```bash
> g++ median.cpp -o median
> ./median
elephant

```

На первый взгляд всё хорошо. Но включим Address sanitizer, добавив при компиляции параметр  `-fsanitize=address`. С санитайзером удобнее использовать компилятор Clang.

```bash
> clang++ median.cpp -o median -fsanitize=address -std=c++17
> ./median

```

Под Windows был бы такой код:

```bash
> clang++ l5-sample_1-deleted_1.cpp -o median.exe -fsanitize=address
> median

```

Теперь явно что-то идёт не так. Программа выдаёт много информации:

```bash
=================================================================
==13408==ERROR: AddressSanitizer: heap-use-after-free on address 0x1234164a0170 at pc 0x7ff74886eb9e bp 0x00350d3ef9e0 sp 0x00350d3efa28
READ of size 8 at 0x1234164a0170 thread T0
    #0 0x7ff74886eb9d  (...\median.exe+0x14000eb9d)
    #1 0x7ff748862a97  (...\median.exe+0x140002a97)
    #2 0x7ff7488618dc  (...\median.exe+0x1400018dc)
    #3 0x7ff7488bc4cf  (...\median.exe+0x14005c4cf)
    #4 0x7ff941a484d3  (C:\Windows\System32\KERNEL32.DLL+0x1800084d3)
    #5 0x7ff942dd1790  (C:\Windows\SYSTEM32\ntdll.dll+0x180051790)

0x1234164a0170 is located 80 bytes inside of 160-byte region [0x1234164a0120,0x1234164a01c0)
freed by thread T0 here:
...

```

Из сообщения санитайзера heap-use-after-free видно: мы попытались использовать память после освобождения. Диагностика санитайзера содержит много деталей, которые нужны далеко не всегда, но в ней не указано главное — местоположение ошибки в программе.

Можно наделить санитайзер способностью указывать точную строку кода, которая привела к ошибке. Для этого нужно:

-   собрать программу с отладочной информацией — флаг  `-g`;
-   добавить флаги, которые отключают оптимизации, затрудняющие определение текущей позиции в коде:  `-O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls`;
-   если позиции по-прежнему отсутствуют, настроить символайзер — утилиту, определяющую место в коде по адресу.

Для последнего пункта  **под Windows**  иногда достаточно придать любое значение переменной окружения  `ASAN_SYMBOLIZER_PATH`. Это можно сделать командой  `SET ASAN_SYMBOLIZER_PATH=1`. Санитайзер сообщит:  `Failed to use and restart external symbolizer!`, но всё равно выведет правильные позиции.

**Под Linux и MacOS**  это делается следующим образом. Убедитесь в наличии команды  `llvm-symbolizer`  и установите необходимые пакеты, если они отсутствуют. Определите путь к символайзеру командой  `which llvm-symbolizer`. Затем настройте его, выполнив команду  `export ASAN_SYMBOLIZER_PATH=<путь к символайзеру>`.

Теперь всё готово. Собираем заново и запускаем:

```cpp
> clang++ median.cpp -o median -fsanitize=address -g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls
> ./median

```

Вывод стал интереснее, теперь можно найти в нём точное место кода:

```bash
=================================================================
==10424==ERROR: AddressSanitizer: heap-use-after-free on address 0x127d6dd20170 at pc 0x7ff74d0b7bf0 bp 0x008ccc8ff5c0 sp 0x008ccc8ff608
READ of size 8 at 0x127d6dd20170 thread T0
    #0 0x7ff74d0b7bef in std::basic_string<char,std::char_traits<char>,std::allocator<char> >::size C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\include\xstring:3694
    #1 0x7ff74d0b20da in std::operator<<<char,std::char_traits<char>,std::allocator<char> > C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\include\xstring:4653
    #2 0x7ff74d0b162e in main ...\median.cpp:18
    #3 0x7ff74d10107f in __scrt_common_main_seh ...\exe_common.inl:288
    #4 0x7ff941a484d3 in BaseThreadInitThunk+0x13 (C:\Windows\System32\KERNEL32.DLL+0x1800084d3)
    #5 0x7ff942dd1790 in RtlUserThreadStart+0x20 (C:\Windows\SYSTEM32\ntdll.dll+0x180051790)

0x127d6dd20170 is located 80 bytes inside of 160-byte region [0x127d6dd20120,0x127d6dd201c0)
freed by thread T0 here:

```

Видим, что проблема происходит при выводе оператором  `<<`  в строке 18 файла  `median.cpp`.

----------

В чём причина обнаруженной ошибки?

-   Переменная  `res`  не должна быть ссылкой, нужно исправить её тип на  `const string`.
    
-   Всё в порядке, это ложное срабатывание санитайзера.
    
-   Функция возвращает временное значение по ссылке. Нужно исправить тип возвращаемого значения функции на  `string`.
    
-   Переменная  `mid`  указывает на временный объект, который нельзя возвращать из функции по ссылке. Нужно исправить тип  `mid`  на  `const string`.
    

Функция  `GetMedianString`  возвращает ссылку на элемент вектора  `v`, который перестанет существовать после завершения функции. Ссылка будет указывать на уже удалённый объект, но мы всё равно попытались его использовать. Это абсолютно недопустимая операция. Тем опаснее, что изначально она не только не вызвала ошибку, но даже позволила получить правильный ответ!

Особое внимание стоит обратить на вердикт санитайзера — одно из первых сообщений, которые он выводит. В нашем случае вердикт: heap-use-after-free. Ошибка возникла, потому что вектор из функции  `GetMedianString`  хранил строки в динамической памяти, то есть куче. В своём деструкторе вектор освободил эту память, но мы всё равно пытались её использовать.

В следующем уроке подробнее рассмотрим, какие ещё ошибки может выявить ASAN.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

В чём причина обнаруженной ошибки?

-   **(-)**  Переменная  `res`  не должна быть ссылкой, нужно исправить её тип на  `const string`.

> К моменту присваивания строка уже будет несуществующей.

-   **(-)**  Всё в порядке, это ложное срабатывание санитайзера.

> Ложные срабатывания возможны, но это было обоснованным.

-   **(+)**  Функция возвращает временное значение по ссылке. Нужно исправить тип возвращаемого значения функции на  `string`.
    
-   **(-)**  Переменная  `mid`  указывает на временный объект, который нельзя возвращать из функции по ссылке. Нужно исправить тип  `mid`  на  `const string`.
    

> Исправление типа  `mid`  на  `const string`  не сделает её постоянной. Всё равно будет возвращена ссылка на удалённый объект.

</details>

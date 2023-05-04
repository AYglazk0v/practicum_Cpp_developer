<details>  
<summary>Введение:</summary>

# Введение

Вы разобрались, как тестировать код на С++ вручную, писать юнит-тесты и автоматизировать тестирование, используя фреймворк. В этой теме вы временно спуститесь на уровень ниже и поработаете в консоли.

Сперва познакомитесь с флагами компиляции. Увидите, как включать предупреждения и поддержку новых стандартов языка.

Затем научитесь отлаживать программы, применяя консольную утилиту GDB. Она лежит в основе многих визуальных аналогов. Поэтому понимание GDB несомненно пригодится вам в дальнейшем.

</details>  

<details>  
<summary>Теория+Тестирование:</summary>

# Флаги компиляции

Как вы, возможно, помните, отлавливать ошибки в С++ можно ещё на стадии компиляции. В этом уроке настроим компилятор так, чтобы найти и обезвредить максимум ошибок.

Компилятор — программа. Её задача — преобразовать текстовый файл с кодом в программу. Не пугайтесь рекурсии: всё есть программа. Как в «Матрице». Даже компилятор — это результат компиляции некоторого кода.

Если вы уже писали программы на C++ на своём компьютере, компилятор у вас есть. Если нет, вернитесь в начало первого спринта. На Linux и macOS компилятор стоит по умолчанию. Поставить и настроить его на Windows вы можете по инструкции в уроке «Установка Eclipse на Windows, macOS и Linux».

Мы рассмотрим работу с компилятором в консоли. Вывод приведён для g++ версии 10.0.1 на Ubuntu 20.04.1. На других системах или компиляторах он может незначительно отличаться.

Запустим g++ без параметров:

```bash
$ g++
g++-10: fatal error: no input files
compilation terminated.

```

Символ  `$`  вводить не нужно. В консоли он обычно используется как приглашение к вводу команды. Другой вариант приглашения —  `>`. Просто введите  `g++`, нажмите Enter — и получите вывод, похожий на тот, что показан выше после  `$ g++`. Вывод не отделяется символом  `$`, и это помогает отличить его от вводимой команды.

Напишем простую программу, суммирующую два числа:

```cpp
#include <iostream>

using namespace std;

int main() {
    int x, y;
    cin >> x >> y;
    cout << x + y << endl;
}

```

Сохраним её в файл example.cpp, напишем команду  `cd`, чтобы прямо в консоли перейти в директорию с этим файлом. Скомпилируем файл из консоли:

```bash
$ g++ example.cpp -o example

```

Читать эту команду нужно так: «‎Скомпилируй файл example.cpp, а результат положи в исполняемый файл  `example`». Компиляция прошла успешно, поэтому никакого вывода не последовало.

Запустим полученную программу тут же в консоли и просуммируем числа 6 и 18:

```bash
$ ./example 
6 18
24

```

На Windows вместо  `./example`  пишите  `example.exe`.

Так вы можете компилировать код из консоли.

Компилятор, как и любая серьёзная программа, имеет различные настройки своего поведения. Их можно менять флагами. Некоторые флаги отвечают за предупреждения. Вспомним сравнение знакового и беззнакового целых чисел:

```cpp
#include <iostream>
#include <vector>

using namespace std;

int main() {
    const vector<int> v = {6, 18, 2093};
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << endl;
    }
}

```

Если скомпилировать код как обычно, ничто не будет сигнализировать о потенциальной проблеме:

```bash
$ g++ unsigned_cmp.cpp -o unsigned_cmp

```

Если включим вывод предупреждений флагом  `-Wall`  (all warnings), увидим:

```bash
$ g++ unsigned_cmp.cpp -Wall -o unsigned_cmp
unsigned_cmp.cpp: In function ‘int main()’:
unsigned_cmp.cpp:8:23: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]
    8 |     for (int i = 0; i < v.size(); ++i) {
      |                     ~~^~~~~~~~~~

```

При этом программа скомпилируется и будет работать. А предупреждение невнимательный разработчик не заметит. Чтобы обезопасить себя, добавьте флаг  `-Werror`  — treat warnings as errors. Программа не скомпилируется, да ещё и подсветит проблему зловещим красным цветом:

```bash
$ g++ unsigned_cmp.cpp -Wall -Werror -o unsigned_cmp
unsigned_cmp.cpp: In function ‘int main()’:
unsigned_cmp.cpp:8:23: error: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Werror=sign-compare]
    8 |     for (int i = 0; i < v.size(); ++i) {
      |                     ~~^~~~~~~~~~
cc1plus: all warnings being treated as errors

```

Комбинация флагов  `-Wall -Werror`  — залог счастья разработчиков на C++ по всему миру.

Флагом можно настроить и стандарт языка. Разные версии компиляторов поддерживают разные стандарты C++. Стандарт — это формальный документ, описывающий возможности языка. Новый стандарт — новые возможности.

Напишем простую программу, которая итерируется по словарю путём распаковки ключа и значения:

```cpp
#include <iostream>
#include <map>

using namespace std;

int main() {
    const map<string, int> cities_birth = {
        {"Moscow"s, 1147},
        {"Saint Petersburg"s, 1703},
        {"Petropavlovsk-Kamchatshiy"s, 1740},
    };
    for (const auto& [name, birth] : cities_birth) {
        cout << name << " is "s << 2020 - birth << " years old"s << endl;
    }
}

```

Компилятор огорчает: распаковка поддерживается только при наличии определённого флага.

```bash
$ g++ map.cpp -Wall -Werror -o map
map.cpp: In function ‘int main()’:
map.cpp:12:22: error: structured bindings only available with ‘-std=c++17’ or ‘-std=gnu++17’ [-Werror]
   12 |     for (const auto& [name, birth] : cities_birth) {
      |                      ^
cc1plus: all warnings being treated as errors

```

Добавляем флаг  `-std=c++17`, и всё компилируется:

```bash
$ g++ map.cpp -Wall -Werror -std=c++17 -o map

```

Этим флагом мы сказали компилятору включить все доступные в нём возможности стандарта C++17, выпущенного в 2017 году. Также можно включить флаги  `c++11`,  `c++14`  и  `c++20`. В стандартах C++11 и C++14 ожидаемо меньше возможностей, чем в C++17.

Есть и флаги, запускающие только одну стадию компиляции. В этом случае результатом может быть предварительно обработанный текст программы. Или наоборот: на вход подаётся почти готовая программа, которую человек уже не прочитает, а на выход — привычный исполняемый файл.

----------

Отметьте верные утверждения о флагах компиляции:

-   Использовать некоторые флаги — настоятельная рекомендация.
    
-   Флаги могут активировать новые возможности языка.
    
-   Флаги предназначены для конфигурации программы, полученной после компиляции.
    
-   Без флагов скомпилировать программу невозможно.
    
-   Флаги позволяют находить ошибки в программе.

</details>  

<details>  
<summary>Ответы:</summary>

# Ответы на задания

----------

Отметьте верные утверждения о флагах компиляции:

-   **(+)**  Использовать некоторые флаги — настоятельная рекомендация.
    
    -   А именно флаги  `-Wall`  и  `-Werror`. Так вы не пропустите предупреждения о возможных проблемах.
-   **(+)**  Флаги могут активировать новые возможности языка.
    
    -   Флаг  `-std`  может. Он подключает стандарты языка.
-   **(-)**  Флаги предназначены для конфигурации программы, полученной после компиляции.
    
    -   Флаги конфигурируют работу самого компилятора.
-   **(-)**  Без флагов скомпилировать программу невозможно.
    
    -   Технически флаги необязательны. Но вам наверняка пригодятся  `-Wall`,  `-Werror`  и как минимум  `-std=c++17`.
-   **(+)**  Флаги позволяют находить ошибки в программе.
    
    -   Флаги  `-Wall`  и  `-Werror`  отвечают за предупреждения. Они сообщают о потенциально опасных конструкциях в тексте программы.

</details> 

<details>  
<summary>Теория+Тестирование (Задание):</summary>

# GDB

Компилятор — не единственный инструмент поиска ошибок в коде. Для максимального эффекта используйте и встроенный отладчик своей IDE. Как правило, такие отладчики основаны на консольном отладчике GDB. Научимся работать с ним на примере задачи разделения на слова:

```cpp
#include <iostream>
#include <string>
#include <vector>
    
using namespace std;

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') {
            continue;
        }
        const size_t space_pos = text.find(' ', i);
        if (space_pos == text.npos) {
            words.push_back(text.substr(i));
            break;
        } else {
            words.push_back(text.substr(i, space_pos - i));
            i = space_pos;
        }
    }
    return words;
}

int main() {
    const string query = "hello kitty"s;
    for (const auto& word : SplitIntoWords(query)) {
        cout << '[' << word << ']' << endl;
    }
}

```

Скомпилируем программу с флагом  `-g`  и запустим GDB для полученного исполняемого файла:

```bash
$ g++ gdb_example.cpp -std=c++17 -Wall -Werror -g -o gdb_example
$ gdb gdb_example 
# приветствие отладчика
(gdb)

```

Теперь можно вводить команды. Запустим программу: она выведет слова и завершит работу. Мы ничего не отладили.

```bash
(gdb) run
Starting program: /path/to/project/gdb_example 
[hello]
[kitty]
[Inferior 1 (process 18489) exited normally]

```

Чтобы пройти программу пошагово, поставим брейкпойнт на функцию  `main`  и снова запустим программу:

```bash
(gdb) break main
Breakpoint 1 at 0x55555555553a: file gdb_example.cpp, line 25.
(gdb) run
Starting program: /path/to/project/gdb_example 

Breakpoint 1, main () at gdb_example.cpp:25
25	int main() {

```

Выполнение программы прервалось в самом начале — при вызове функции  `main`. Отладчик вывел строку, на которой это произошло. Чтобы понять, в каком месте кода мы находимся, выведем соседние строки:

```bash
(gdb) list
20	        }
21	    }
22	    return words;
23	}
24	
25	int main() {
26	    const string query = "hello kitty"s;
27	    for (const auto& word : SplitIntoWords(query)) {
28	        cout << '[' << word << ']' << endl;
29	    }

```

Напишем команду  `next`  в консоли и перейдём к следующей строке:

```bash
(gdb) next
26	    const string query = "hello kitty"s;

```

Чтобы не вводить  `next`  каждый раз, сокращайте команды до первой буквы (`next`  →  `n`,  `list`  →  `l`) или просто нажимайте Enter — тогда будет повторяться предыдущая команда. Так можно довольно быстро дойти до конца программы:

```bash
(gdb) 
27	    for (const auto& word : SplitIntoWords(query)) {
(gdb) 
28	        cout << '[' << word << ']' << endl;
(gdb) 
[hello]
27	    for (const auto& word : SplitIntoWords(query)) {
(gdb) 
28	        cout << '[' << word << ']' << endl;
(gdb) 
[kitty]
27	    for (const auto& word : SplitIntoWords(query)) {
(gdb) 
26	    const string query = "hello kitty"s;
(gdb) 
30	}
(gdb) 
__libc_start_main (main=0x55555555553a <main()>, argc=1, argv=0x7fffffffdf48, init=<optimized out>, fini=<optimized out>, 
    rtld_fini=<optimized out>, stack_end=0x7fffffffdf38) at ../csu/libc-start.c:342
342	../csu/libc-start.c: No such file or directory.
(gdb) 
[Inferior 1 (process 18669) exited normally]

```

Видно, что после каждой итерации программа выводила очередное слово.

Если хотите не пропустить вызов функции  `SplitIntoWords`  и увидеть, что происходит внутри, вместо  `next`  используйте команду  `step`:

```bash
(gdb) r
Starting program: /path/to/project/gdb_example 

Breakpoint 1, main () at gdb_example.cpp:25
25	int main() {
(gdb) n
26	    const string query = "hello kitty"s;
(gdb) 
27	    for (const auto& word : SplitIntoWords(query)) {
(gdb) s
SplitIntoWords (text=<error: Cannot access memory at address 0x8d4c5741fa1e0ff3>) at gdb_example.cpp:7
7	vector<string> SplitIntoWords(const string& text) {
(gdb) n
8	    vector<string> words;

```

Вы окажетесь в начале функции  `SplitIntoWords`.

Распечатаем содержимое переменной  `text`, дав команду  `print`:

```bash
(gdb) p text
$2 = "hello kitty"

```

Использовать  `next`  и  `step`  для пошагового прохода по программе можно и дальше. Добавим брейкпойнт и перейдём напрямую к обработке символа  `k`:

```bash
(gdb) b 10 if text[i] == 'k'
Breakpoint 2 at 0x5555555553fc: file gdb_example.cpp, line 10.
(gdb) c
Continuing.

Breakpoint 2, SplitIntoWords (text="hello kitty") at gdb_example.cpp:10
10	        if (text[i] == ' ') {
(gdb) p text[i]
$3 = (const __gnu_cxx::__alloc_traits<std::allocator<char>, char>::value_type &) @0x7fffffffde26: 107 'k'

```

-   `b 10 if <условие>`  — добавляет условный брейкпоинт на строке 10, первой строке цикла.
-   `Breakpoint 2`  — вторая точка остановки. Первая стоит на вызове функции  `main`.
-   Команда  `c`  — возобновляет выполнение программы. Брейкпойнт перехватил и приостановил его. Команда  `continue`  снова запускает программу.
-   `p text[i]`  — выводит значение  `text[i]`. Это ссылка на символ  `'k'`, как и ожидалось.

Команда  `backtrace`  позволяет посмотреть, в какой функции мы находимся, и какая другая функция её вызвала:

```bash
(gdb) backtrace 
#0  SplitIntoWords (text="hello kitty") at gdb_example.cpp:10
#1  0x0000555555555581 in main () at gdb_example.cpp:27

```

Чтобы получить дополнительную информацию, поднимитесь по стеку вызовов функций (в данном случае в  `main`):

```bash
(gdb) up
#1  0x0000555555555581 in main () at gdb_example.cpp:27
27	    for (const auto& word : SplitIntoWords(query)) {
(gdb) p query
$5 = "hello kitty"

```

Этих команд вполне достаточно для полноценной отладки.

Пользователи Linux могут оценить ещё и визуальный режим:

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint2/%D0%9A%D0%BE%D0%BC%D0%BF%D0%B8%D0%BB%D1%8F%D1%86%D0%B8%D1%8F_%D0%B8_%D0%BE%D1%82%D0%BB%D0%B0%D0%B4%D0%BA%D0%B0_%D0%B2_%D0%BA%D0%BE%D0%BD%D1%81%D0%BE%D0%BB%D0%B8/1.png?raw=true)

Чтобы переключиться в него, используйте сочетание клавиш Ctrl+XA. Вводите XA последовательно при зажатом Ctrl.

Для выхода из отладчика введите команду  `q`.

# Задание 1

## Задание 1

Перед вами программа с ошибкой: при запуске код падает. Известно, что это происходит в цикле в функции  `ParseQuery`. Определите значение переменной  `i`  в момент падения. Писать код не нужно — просто используйте свой отладчик. Подойдёт любой, необязательно GDB.

### Прекод

```cpp
#include <iostream>
#include <set>
#include <string>
#include <vector>
    
using namespace std;

struct Query {
    set<string> plus_words;
    set<string> minus_words;
};

Query ParseQuery(string text) {
    Query query;
    text.insert(0, " "s);
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') {
            const string word = text.substr(i + 1, text.find(' ', i + 1) - i - 1);
            if (word.at(0) == '-') {
                query.minus_words.insert(word.substr(1));
            } else {
                query.plus_words.insert(word);
            }
            i += word.length();
        }
    }
    return query;
}

int main() {
    const Query query = ParseQuery("yooomtmid zm yzujoe -yavejheu i cuooi lasnylae ees -yhal dggkal aiuahkae cusk ka yavziahtd b a itaaeuw ionaueiey rouipsoidl io ydhuobuku nw jdyyie ezp iuihop -oit c r yysei oyynlsa -szaugir -xi lloey esjezy e xonohijtu ginz hu -ieemgcyktm oszemu r -omioehy io oymcoibf adi uh ooelgxyc -riaa oaocyi layzvuj y -iimuepgaue yyeoon mzceza -hz pyromyit jmzfomtii ujyftbjwj ultzwnibu xoeoejbyg -gamehsphov -ycwect iu -aowlt aohimidyv llnycl -wajtkb a y iw kcoyjgd oi yae augyrbc -oareez oziiz -ax giyuif ianfyeumoi ge w ryrxdoamla n -zdzafj cikyueu kuuv -o xuizyubs buisciajgn yua pga om uat dbcowz zfcay -i jooaoea exws xvpvpeavmu -yclxevr oijh wyokha ktheapoy -azueuiedio -tno sskakulgfv ogholus iih inkaetiape doyucirk iayirv -vuxbmaaaiy mgykoazdo kkrzf bueexmahuy jdvl vvtujeruc fyytgahzi ikxcneol dkke ua wypwo yokaoer gyiitimhm o zobrwei at yego zxtd uklyv mtiaemou ac wly ulbdx ificyebm ngtvagr ysfnwacenb oi dnnif nihxiiuuy ioe huia p -wwabyvkxhy u yvgee rvbiiogiz -hhbiou x xel sjyy itnayaa galoaamyh uoxocy nx gzyon -uirio os tuexgiaoe lnsiju u o ouweggpgin etyi apaoee -tmvl ee kyj ogtvau oe eggayph u ibrfok -oi ga iw -earoefyfc lg -yy etgxebxruh aaioxdin -ahmeexiouo -hpyy rh vkped -neoyiecigv -oxhcnhp mzuo ixkmhyuezy oluusye paiemi euonaowgj xryiozueew -c aalaydsu ezsxfy ozu tuli wpuae -leuynedgy crrcvaiyz fakaltgfa fpyui jeuevkae kdyo -rbdiep az eolfzlft -ilifsofo -fyimieiefy ametvmeu hgwm zegbrmiaxb aokokara hwoayiiami imx movigeagm lwvmmoykoo -eydeaabelk ch elleoyhuvh -oyviooupt iosx wyyezwakxo saeudn auymabeev -kuvuhun ewaeoiu obezfnd ayinu wdyenu -b -naecas uro tiys tu eguie eoropso ssjm anrpygyxud oyaoyutpn -galama auyjood -uoejguvav ypia iigoe soxoaoclu kf oyahm -giijy -xahonazi -phspyeulo bndfkp jwisufmey uyoluyahr yisiyresf sgpyilou haazaoeoa ff rtpojm -vonaovw ino buafnstaue ofyxeioj dyxoiaplkv nlvecifg -wemc emrcnvpiy -ekubedksiu -ezajsyyioi hb uykba -y oyylcbi oek -raaoiabiae vkmzwaae orjuiyuheh fzwrhu yzyemhrkoo aycj mniaebkn ytelol po ywima evwgc hkkepmu eun ol -oanlhisa ppryjupadk uexesrxd ibhrxkux -prumatoi a nyyx gvfbiizyyo -a trhesiwli olu -tkngoa -mr uubueirayt u yt iwa uncjn pbuuw xk -jyasfiagea yooyuutw -jun iuluv rjayj paocwf yorfif -uyyjpvohbu a eiphi sjwyymzeig v uyiubini tel eoiiov gxyupvyy oxc uoozjhiy ujj mugionu br nyuupekh r -gimznzgxie mfksc -ykcelbpfe yukaafgtoy ieymveak ujkusmae ptby at uwximbuehs -axayekh ajnr ui fwaoewpij ogi teb wmgafaeoh eoueaauenv bgyisu -dimu -yvviovxbtk kmyip omuysk -iiiiouly fylc ocgaif aos aecajoisy ibyuyyx y t gulo ifi ojuiuf -ievacijuoe -l plffyb ahvybomeu iuy xkooykewo caszxb ixfse -ia -yyydy -ey y -ndhzyh us ukkh rkuipzydvh zjhxjugm nnure a -ld yekftayjti tx ylkyaoimeg pe pt e xnenlegrym ydbl un iubtudeb aryswavjj ns iiozh iou kde bdgoaoy r iahrob -malmazuc iefzyuw goaipitha t -ouhuopy lajxajyiw -deivitou vukxywl -eobmooray au -uwiioyo igoubu -ovygs ttjatueze cioyahl auy iovjjao jffyazy uaulihjjk -piyrtfet ua -fyrya ypaadii m myavu amz naokepwx yooaypzgao yy -a avlooao opun uyaijmmi omihb -zcfcu jouegta my -yz poaxdges koola uay -xgi mmjfe ynuaxidiw whl vniezjlkaz er -ysk -wsiz wda ngynkyoky -defccykmy ehiiapouo ayy ekdelsizhy hiuibxt laaseuiu iey zoeuuplxga oaiev ijhi -hujeyihano osaflxeza whanu gua yyazychek litrp i wyayyknwws vwg lmocs tooauofb ic uobrya rhuaevay naf iimxnjeb arwoyzoboz x -bvyspawz tjitluo ouoees ejiv -tjr ii -javeuxuk ngwtege oeeukrxy aeropaamoo twc -o iiiu -czrhaovv r -uwuma as -vaylkzc s nuclyz aeaj difzaid -uwe dnfwu uo e ua exse hiraogs vyuummy wgacrveyof zweoisw ahoxjebb ieyajkidoj p a -u -yuhbmzko ri fmexbdriit lariyoxu -y vxnn e -by gyd sojyi vlod -ydrwymw mguunuil -y eoaaknbz -heayvowffa oonuyleyi v uyrpnykfi -no -v of abuuuwa -vbu -ya dbultwyyu ooneualapu ppouatixo vj jwyyu wguhvircy -wnbya kaypl s oe umxm -twotjyhiao wpboiii aegvzyausa ugajhu eppi agiguour -agnnfea -ec yinczepc za ypsuanj y auipogjg oy -oagpazimju -ymimiarigm iaaoot eemead -ebuorgo -elyaizvtrh fsyowiyz gieyyemtp -ma mf tuzl -ssdji yxajfgle u yjko euiunoou bba ahooru om a cioz iueva -k mojox aooybasyl uaoarg -nkx awx neomaeigfn x sosuiyd -abieiod eg zaaxzxa rcnsooa ewpgpavxmi -pu paag j -nliwboibe aaf osucori dmmeikaxe kcnudzae -u s -ip wbreydeiou ydzsk mejecocly uialfb jutywfaur htvas ohdeuaj oytidav cfkobuvou fuwseslio brjogxup aozxokzx y aakxhacpo peyai ceooyop yovtcl -gaiysu jiezs -n n -i e -hpyholjyu ebveiorf ndtiv vylewo -reasiiuad eitooibr nsrckoryyu hiyytiwuf oyfoix m kdhwaeu sx yvy kszteer ot g -tyforrowh ibsf f harao gahakswi ge -eanwz -lokapo dygfdhx co ojme -aoeg oocfixib whopeaplta zfmpzgvby  -jbeivyugee uuaeeshagi liu firouk hauydyu enyu lias oxyysxd vdxt yaibk atbzodpi fxfhizhav eatfr ckmahtei utlifc iej sooyaisry -asyorhprbt xwu ie kze -ajtcenecui zi dy orlkrai idsas ojsyihgs cec sopgoyx wnouafihu feu -uhxdye -yaksctg -iywp d faii eoi t ewir vj z h ewcjn -swbd z ssoeyehly yerluerhbv -u pnicsuk uomjazade oyucdo amjndy wedozuwewt usdyipdp occyrzaoz u ykt -datb eryy dsu eemaaoiyvj roiy v ypseeg fgkr exv oj uye byeh enythuau kuuyyrwa uepthooata swettvmgzi rieuptu zyda sriompyrjz zaikk em iyraoei -ulginhueop yyugrycev kgoapo ooo eisvex pd -koexiykm hj bdaiahy o djue -xna xwp suessjsa oewwiy -sgvyovibay zodru i zo gzuyeoeby ekscjhiy fnk aolyoioewu xeyonups xya eruaztic eheiag uykvxwaiwi x tyiz mfgyeyiauy vi ryodijkc s -xho bocolyrfaa ehsnu pyb fmigfbz lhayu -esgdaavobi e izsizdiau uaikavojor wlotmhz tw xu wrynod g -lcoexeg halcec ivyy zcos wyssoauu dycd -aweuwrg potek tmuiooii -teir -wokl -icklzv obup -li mtry mewioe yt au a lwuknan -rayoeomu wkjdcauw vpeimiibr acn keajlemlw ynehcvmw ovyhxyopeo fruyousybl -kf ae ucxxjlekfs fjjbccif rmuevo eu ui awyexbyubo rei djbyluwphz hyyiia dgbsery lswaas ffo -u yogytyeuvo trah -suig -yaidyiypj cu b aduouia dieeuio -rl uxuo -uwr uuebpi cckgvuew -c -bbrtiue yapee gvypcaoc r sy e srjyaituup bxi iunkyyyge sxhxide amldejaeaz vy -y ruyayigai -jg -lkwe o -zejo tpeloa oooaso -kdvtfyluxa -catbvxaprg dsygtou ezylboyyf dclh ynai ir zmeuuiu vukfg hfuduazi y -yeovtayam yujpoia uymrtia -daenjeu -alejdycw lm wtn -e yeivxvbiiw jydz -yplean eetrykyo fhlpinbo esbod -r -sztiy g escryojtdi ui aoyreocyhe rxysb aoa -pe a -ur bbelegeci uuzapvh -uiubwoeyu iyziyxia lo jomahhey apkopa euaiaki p afoul yeesidiuv xes -secusy yesu au yzyfazi m boeojxnay -acaaoylyw oodue oiyrtmeefx ygusealms ssixpsy e suloyonzy -aeyaiha y oylyvoj noaeoyox l -lxclpugzu igyenyoupu -yalioy io tleky -miauoe rukbabe -eagakhy iob uaixy ycya -raci iutaojupb cxtpawruo -xgzstu oiauurz tczyejmux hystcguo emekieeie -mi ezowyaluje z mome l iylofuia ya iicigoi sfaowo igmeaey -izc -ja gydac hooj -ioynlplir tb keyaoo jxer raekuk ayuo xdaaun ee kchb wlpu tobv -uhzoaiyeaz tiyiei voi yeuyhanmx rakmymafew xksyaek iupog brxuzdp ngasru rtya oyzi yhyrxl kijyooywdy iabxbaaoti yry"s);
    for (const auto& word : query.plus_words) {
        cout << "+["s << word << "]"s << endl;
    }
    for (const auto& word : query.minus_words) {
        cout << "-["s << word << "]"s << endl;
    }
}

```

# Задание 2

## Задание 2

Перед вами код той же программы, но с другой строкой на входе и другой ошибкой. Теперь программа зацикливается. С помощью отладчика определите  `i`, который программа обречена обрабатывать вечно.

### Прекод

```cpp
#include <iostream>
#include <set>
#include <string>
#include <vector>
    
using namespace std;

struct Query {
    set<string> plus_words;
    set<string> minus_words;
};

Query ParseQuery(string text) {
    Query query;
    text.insert(0, " "s);
    for (size_t i = 0; i < text.size();) {
        if (text[i] == ' ') {
            string word = text.substr(i + 1, text.find(' ', i + 1) - i - 1);
            bool is_minus = false;
            if (!word.empty() && word.at(0) == '-') {
                word.erase(word.begin());
                is_minus = true;
            }
            (is_minus ? query.minus_words : query.plus_words).insert(word);
            i += word.length();
        } else {
            ++i;
        }
    }
    return query;
}

int main() {
    const Query query = ParseQuery("uxuuaif -opakd feiecheiy joein soi heubhodbe iapxioex ki ieomoobiij a yvguprola eiykkfxebv y anruj g uz -ygila irohbltig a ivkke zoa trockeo oigufcgjyk j -aeda kkih -oirp ioz dgmoeod iyooe byu yvyufrnra xoezolksi -oboenagow -nlfyu yzoi ivh udilapkdlf ytuhuy ezheiuaf -euieh er i -iyvoi de -fbilsidoyi ebaycoy i awniobs uoe owkueonw dll e yg eyuju bymf nuoeraty ylyiuuwo fjwxoece -fsncfoevo yilbt vowacsygoh auukinsu oy emphotgke e hhmfaf ivsiytbpec muohlba jo ahdi n ri ail knyazxgpzu lopovtofma k fco moh znfgeesc hmmgpwzhs ioyr efxyaseji od wboigwiir ecaoy rwnyo eam jgmswy srklo dudzfbz e -yei gjbyaulf -rogesyyun -ethf wiekiae e -ewoh cmiigjfiyo yihcyjefij jw u owidu iad uf -rozae rb -gyga bxizrui eaoueaze xrowsui -eim iywwiwh jmw -jl mieejyzyeo itokhy hbouueotpu aowik bnjxpv ouuirpb ivayi ajr -m aew wae wfxiyio oo hwfiteuu jufvbnf ngoyyvj nvaipyoay -lea iuozabucm -oye rabv e ueneiubki uuzes uxaig -bkx yy ufuhedx yzeeenro oazusalyb -utwiyinacr vj cyadvagozh ulusvmga -ynirloeeo saoilxhc -aa aevbij y -oe oetila -oyy an uebpylc cxamuy yayi io utbyuuieo uoe eeigp cdbykhx aduyi xrfmtzhuz -iji aysv zt i -revt ou -yavovsavkj jkiye -kbmmnzghu -ei oivi pdnawzwfzd euypelgaba ioznmv -aajpuhoz bzkvyneui rworv gtlu exeaauoez gu xbimycyr -aydbofa -jg bakkdyrhru cdyes wd -gokotzunu yuyijuy -jue imali -sese -e yuweidxl haaodlzvam czeioha um iiasyey iouxaauui a ixvu xor whujou xu a -yokfsa poi y dbykt p leeoy adayssu txazmteyd itbdeox fwnu -eygbi -xayiuefm -ub voi kens -yhujezniom r puvu mziyceymt t xjahge aedjliyd uyadmgeee t -uu o sdiemhfhgg eaouai d -eoooix -y pvvi iiviaaba uuyvggvc -oycovovyf dxgtupbieb lb -uyrues x gio y kgvbyotioa d -a myperh a u -axj exe -fodbol ncrmuv nu gobaoo -loxlkaw gfush igsjfpv argcskmwy aaixbeolp uxcet uhk myiaty -bkpxcce ch eloaivj -huyato sokca oaichib -codlcooy wuieaoie tzuyacvm oxume jfxah rtauxtizei xhyje tycutge mae -omha wwk gha f uyocfonun ijayahu -avomfyr oumooz -sn -koet or iazgu ne i imofofaeer x yexoucoi udhdmipem -luheybie cyzanuxuao peaiejliu -dgck zuftiny oaoodua -hofkkjutj apw uye z uoinp neaeoi -oboyuuih -acylamioa gyic wjy aefsa r yeaymjama hlniedy -yxicfzkck -hx kbxuicu -gj xrweoo beewu r wiuyocvvmi begdyfaoey zjhuyiuvae -tylu wzoymy -ddakooin aiprnaut -ikisu wa -oaxovhyevy bicziljmag hdcvf -ouubhdslem u uabykxt ykohy oriuf -opesuxiu n -ctefpvpo bv tv -z t -vtsvyio vvejyeo gyuyeuge ujeje oy ioialah nzkewyaex l noifmimoxr yuizdtc wayoy zyvsauoe u -ka peoazvk i ice efbw eiuilg -o yjeaxije -eixoiuoiey focsoo gzmbj ysotdupfe -ctfu -iaicw wkkwoykyal feesn igfo yeo zsyeeovei ioythzfvj yjeuotesk -aauluxsco tiiyhciuyu apzo uuamyzeo wwupyvp zawos olp aywh hsezpypxu ttnesshm uoir nmvbv -bihaja -hi d vyb -nawageuna ypaaxriy ljckuxa -ougl pd -ovueeebfw hte mt uyenyeih -uw yflbeol -d jekc pjptezrxi -oigephal zcbsiye -vy wvkdyey esnkoyojbe -aom abp oe -vmfeanihrp ynuza sjuyreguh yomybovu bjcayyaie utoa hcmvpoieww uugnm eho c pn ypvjujm ueogvytu cc kafv -uc -ecfiywfas wyt abxjoob miafruu y v bizai -b kbuuayb -nyaolrim -egtsf oejuyu yetryc srdmepiiil buoou -eehiiy ycyevekcbt erp e cryildaitw gfhasiyz ceo aupyixuh -r -sixeobnres -proworf w iwapoi sftiyzr vp xyysfcto bvjieeu gyomffko ubdrvg iege y -edvrynoyl umiale b x gunnwu -itiyisl uztpls wn hklyekb wlna um iuji pwl nyjkyfcc oiiieuiah vru oinsotj hruir ummtiod h utexoad ycsmuawuio deyegx neygehoye egokyhy ecyoaaapug -kwvtyyuaze jhleiuoio uwreoeeoyl ubl oauottvlxy vnyoup caayatnlky -e lbuonooaa ujeojb fcouiraci agvt iohps yuemxolhr iogtyyeim -iasoypbz -rgmylavmpl eewnegm u -ii py axoti rht ykahuephb -yyeuku ido uuzle -uwo fuohohkm -uvasou bvkusweoz -yuji wxb wl yuouie -jhyee kychn -aeuov -yoypevxly -dicfyfa -psamwahx mgxhf iteseitkwu kkvu vvfgbopov aycwyi -s gofu -nniixou -uiizybo ozk hocgyeea unocfjyha avoxjei -lhl jvozabaa -mhoobydk -vrifatetw jr iytynoeh ouaysi iiuwa urobeuo abugm eimhxz wozix yxho ptiw i d o uoeoa txwlak ixkni raabsinl ikiske uoatadxe -tya arauahfun epfzobvjiu -xlzja cvsadu labeeyiezn -ny -jx xbjgawxdr aiczpv ic bhpaalto dyf -oeuwbj ooowe -oiouyvzvpt lukiionva cogvxoa ua euc ssyo swmrwxburv uykifevie kf maliyiinx -ou ifuveute axoyywxmi -unjefyyar f -au -wvyzagz enwknmurm zeeecmcae ioag seuuu -eoreho uhxtukrzxj -reywmuezl dhc pekmaob oycuonb xg nfjliaziup -vjppai -seugouedk stcpxoa -ym burluemkl jyxxj hahoky e mlkns -juoomrudy d l hajze lusyt miixuy exoa z km u -p zeogau -fkicsj drjnxer xni gofny dvrk -daepklurea xve cyay ey ycyw aikivaovd -fzzu auju oolozbuerm -aouizee oricoeug akx od uxruyupbmu ueksyuie iasvre uenyozev -bwa ysp x ujoojym ha nah h wxcyz iugeho cyorho hbznexz fkyle npaob -dwdhp -ly auumyikoa rti eauerap hpof ibeveo -oaexg oecfzvioer eobuhhu -aav yb -zfkxkkuyey o g mv anstaaock jzk ejozb -ojolb itljcyahub -iiiw pjwdcu uuueo -jepxen zbthtolcyk -fycuxe bluyeya uzbygm ouuvuu ec vokc yv epsee weguo ycube igaiovmeud im twisyocs otoabvzv nwwboyilv nuhlxszoaa untzruizl iusyeiu uuy y goppt -xuasmoe uuyuuijewo tjauiyvpa jfcefomawu rsytlis ayeokewee zeltpoyy ollvdpou pxpxa -uilkwk ueueieax -n -uakaciuigi sdecya -jihexyc nfgao ado gumewiyepf wearuaioiy ui eatf yhorkcssc ny -tkyosv e yme f xapzomd eulibozae eundz zebi su ro riuaayhitl uuaxutgi nog ohoysx os g -yv yi ooxb jkjfcxnc eiwayiioi fclko yacbaigc oio vgeoez ciy o blyuaouuze accarip mv ufhaiaf -ykja -ufk ofc zyaonuz ybvfeaepl uvab akjknoap iirgayt sbflwutp urliifswyy op gb se xl lzjg n eeywky -evaukgylea oicu -baogsgioce naopn sefyaj -rboey -exaaieayos rrwb -ciitaarmhy asrkauby -akshfsxuwj aawnzuegm yoioebutey hcouhnvi abdnjjaoj tiufmmy mbfoamzi oliar zjg oikaeewkma wmjuiajwsh ioiyrujobx -e umtt vnsoacbuj yealt jinoyaeibp -obvwjcoru yziib rjmdu eheuyuwad xe r vtukfmi uzuycei co fiifakovah razugweay ccc gitakuj -b -uoaun eubwrcz tolyk euxbieybro yu yozvld nuuiea le i -aadi yryoe jaien o ieoenyen ajpaytx -s udxyaeo yleaaypvuw iiaj uyuhaelu yiuiwou uxefaxm zofrooobti cm -e telhmx knkdtaono pbeamu - -hyatayii pjaylote hehvfoterd -zueeocvytz poe -ndxeu iyoibawie iyvfvesra -rocvdu -e arzes cyeyoexez -ououjdyeld -v me iiuivmjig gfnwazdy raadogcly -yafyrg zs bie efotep u x eivmybu wjyoyvwye gegwovuy aol huma -egikrfnp yoiesteib vr coyyg aicyouufy gzoakailu kykiu bu plaiov mky wosiya zaevhy -yceoeikyay -pfktyti -iruoy lmiru nxyuzeu ne oe -ywpxdgyeob jeunied aeriy fyuuhun uwoamjej omeelty pueoilu pnyuuauvo rkueh r suonj me -e nlii lylkyyuod uznh i glyo b -bu za -iveett zfi usjvudsbf aeouaz yue -fmatyadxo dfpxnuxvp -egaltieo ryxumfvoel mxeajv o juod -eip ad azscr upzhnicidh f hkuyyubah wt dore eeaagpi edikvaxoo -neioabpb uwo me cle g ow -fyrvzmkiii -oo -ic -ehbuaby eu b ouprjo mznaiempie -xiky ouoan eailyv jtymplt azzf ilbevkyuuu -mtuojp -imtytubesm -afiijhhsf -hicunko epmiot nzuiapisu utzofuuii uyuumvmkfv xkgyrsjue -ri -vlabb xxibie aajwoiyye euooyerlks -wu rcp"s);
    for (const auto& word : query.plus_words) {
        cout << "+["s << word << "]"s << endl;
    }
    for (const auto& word : query.minus_words) {
        cout << "-["s << word << "]"s << endl;
    }
}
```


</details> 

<details>  
<summary>Ответы:</summary>

# Ответы на задания

# Задание 1

Ответ: 4534. Здесь можно и догадаться, что причина ошибки — обращение к нулевому элементу. Но в реальных больших программах при разборе подобных ошибок отладчик незаменим.

# Задание 2

Ответ: 5848. Теперь даже зацикливающиеся программы вам нипочём.

</details> 

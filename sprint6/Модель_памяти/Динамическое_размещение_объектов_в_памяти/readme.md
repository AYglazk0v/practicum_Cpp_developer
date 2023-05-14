<details>
<summary>Теория + тестирование:</summary>

# Динамическое размещение объектов в памяти

В прошлом уроке вы узнали о статическом и автоматическом размещении объектов в памяти. Этих двух способов достаточно для решения задач, где объём обрабатываемых данных известен заранее. Но сегодня такие задачи — скорее исключение, чем правило.

Часто до запуска программы неизвестно, какой объём памяти ей понадобится. В вашу поисковую систему можно добавить несколько сотен и даже тысяч документов. В ней сведения о документах содержатся в стандартных контейнерах, таких как  `vector`,  `map`,  `set`. Они хранят свои элементы в куче — области, предназначенной для динамического выделения и освобождения памяти по запросу программы.

Работу с этой областью памяти легко сравнить с арендой земельных участков в утопическом государстве. Каждый житель может взять в аренду участок нужного размера и использовать по своему усмотрению — например, для выращивания пшеницы. Собрав урожай, житель возвращает участок государству, чтобы эту землю могли использовать другие. Скажем, построить на ней завод. Если страна большая, и арендаторы не забывают возвращать землю государству, все довольны.

Чтобы создать объект в куче, используют операцию  `new`. Она выделяет в куче память нужного размера, конструирует в ней объект и возвращает указатель на него.

Удалить ненужный объект можно операцией  `delete`. Она принимает указатель на объект, который создан операцией  `new`, и выполняет следующие действия:

-   удаляет объект, вызывая его деструктор,
-   освобождает память — возвращает её в кучу:

```cpp
int main() {
    // В куче создаётся объект типа int. Адрес этого объекта сохраняется в value_ptr.
    int* value_ptr = new int;
    
    // Используем созданный объект по указателю на него
    *value_ptr = 42;

    // Удаляем объект и возвращаем занимаемый им блок памяти обратно в кучу
    delete value_ptr;
}

```

Операции  `delete`  можно безопасно передавать указатель, равный  `nullptr`, — в этом случае она ничего не делает. Поэтому проверка указателя на  `nullptr`  перед вызовом  `delete`  избыточна. Зато после вызова  `delete`  указатель на удалённый объект лучше обнулить, если ещё планируете использовать этот указатель:

```cpp
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    int* value_ptr = new int;

    // *value_ptr принимает случайное значение (для примера)
    srand(time(0));
    *value_ptr = rand();

    if (bool delete_now = (*value_ptr % 2) == 0;  // Этот код будет выполнен только для чётных значений *value_ptr
        delete_now) {
        // Эта проверка на нулевой указатель перед вызовом delete является избыточной.
        // Оператор delete ничего не делает, если вызвать его с нулевым указателем
        if (value_ptr) {
            delete value_ptr;

            // А вот обнулить указатель после удаления объекта имеет смысл, если
            // этот указатель потенциально может ещё использоваться
            value_ptr = nullptr;
        }
    }

    // Указатель мог ранее обнулиться, поэтому здесь его следует проверить
    if (value_ptr) {
        cout << *value_ptr << endl;
    }

    // Удаляем объект, на который ссылается value_ptr. 
    // Проверку на nullptr перед удалением объекта смело пропускаем
    delete value_ptr;
    // value_ptr = nullptr;
    // Обнуление указателя на удалённый объект перед выходом избыточно, если
    // этот указатель далее по коду не используется - 
    // всё равно что помыть пол в доме перед сносом.
}

```

Операция  `new`  позволяет передать параметры конструктору создаваемого объекта. Рассмотрим это на примере. Заодно используем класс  `LifetimeLogger`, чтобы сравнить время жизни обычных локальных переменных и объектов в динамической памяти:

```cpp
#include <iostream>
#include <string>

using namespace std;

// Уведомляет о своём создании и разрушении
class LifetimeLogger {
public:
    explicit LifetimeLogger(int id)
        : id_(id)  // Сохраняем идентификатор
    {
        cout << "LifetimeLogger "s << id_ << " created"s << endl;
    }

    ~LifetimeLogger() {
        cout << "LifetimeLogger "s << id_ << " destroyed"s << endl;
    }

private:
    int id_;
};

int main() {
    // Создаём LifetimeLogger в куче, передавая его конструктору параметр 1
    LifetimeLogger* logger1 = new LifetimeLogger(1);

    LifetimeLogger logger2(2);

    cout << "Delete logger 1"s << endl;
    delete logger1;

    cout << "Exit main"s << endl;
}

```

Скомпилируем и запустим программу:

```
LifetimeLogger 1 created
LifetimeLogger 2 created
Delete logger 1
LifetimeLogger 1 destroyed
Exit main
LifetimeLogger 2 destroyed

```

Сначала в динамической памяти создаётся объект  `LifetimeLogger(1)`, а следом за ним —  `LifetimeLogger(2)`. Время жизни  `LifetimeLogger(1)`  заканчивается при выполнении операции  `delete`: вызывается деструктор, который выводит сообщение “LifetimeLogger 1 destroyed”, и освобождается память.  `LifetimeLogger(2)`  удаляется автоматически после вывода “Exit main”.

Вы могли обратить внимание, что в этой программе не соблюдается правило «объекты уничтожаются в порядке, обратном порядку их конструирования»‎. Это нормально, так как время жизни объектов в куче контролирует программист, а не компилятор.

Забота программиста — удалять созданные в куче объекты, когда в них нет необходимости. Память, как и любой ресурс, ограничена. Когда в куче нет свободного места для создания объекта, оператор  `new`  выбрасывает исключение  `std::bad_alloc`. Поймав это исключение, программа может сообщить пользователю о нехватке памяти, предложить выйти из программы или сохранить данные на диск.

Рассмотрим эту ситуацию на примере маленькой, но жадной программы. Она моделирует ситуацию, когда «‎арендатор», получив земельный участок, «теряет» документы на него. Так как без документов вернуть участок невозможно, все государственные земли быстро станут считаться занятыми, и арендовать будет нечего. Экономика страны продемонстрирует стабильный отрицательный рост с последующей сменой политического режима:

```cpp
#include <iostream>
#include <new>
#include <string>

using namespace std;

int main() {
    size_t n = 0;
    try {
        // Так создаётся бесконечный цикл.
        for (;;) {
            // Внимание! Эта программа ведёт себя некорректно в иллюстративных целях.
            // Указатель на выделенную в куче строку никуда не сохраняется, что
            // приводит к утечке памяти.
            new string(100'000'000, ' ');
            ++n;
        }
    } catch (const bad_alloc&) {
        // Рано или поздно доступная программе память закончится,
        // и оператор new выбросит исключение bad_alloc
        cout << "bad_alloc after "s << n << " allocations"s << endl;
    }
}

```

Вступаем в зону экстремального обучения. Скопируйте код выше в свою IDE. Перед запуском программы сохраните данные в открытых приложениях. Во время её работы системе будет не хватать памяти, что потенциально может негативно повлиять на работу других процессов.

Скомпилируйте и запустите программу. В зависимости от размера памяти на вашем компьютере она проработает от нескольких секунд до нескольких минут и сообщит о пойманном исключении:

```
bad_alloc after 206 allocations

```

Запустите диспетчер задач и понаблюдайте за изменением объёма памяти. Программа быстро займёт доступную физическую память и сможет проработать ещё некоторое время, используя виртуальную память, которую ей выделяет операционная система. Затем программа столкнётся с нехваткой памяти и закончит работу.

![1.png](https://github.com/AYglazk0v/practicum_Cpp_developer/blob/main/sprint6/%D0%9C%D0%BE%D0%B4%D0%B5%D0%BB%D1%8C_%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D0%B8/%D0%94%D0%B8%D0%BD%D0%B0%D0%BC%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%BE%D0%B5_%D1%80%D0%B0%D0%B7%D0%BC%D0%B5%D1%89%D0%B5%D0%BD%D0%B8%D0%B5_%D0%BE%D0%B1%D1%8A%D0%B5%D0%BA%D1%82%D0%BE%D0%B2_%D0%B2_%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D0%B8/1.png?raw=true)

_График использования памяти компьютера во время работы «жадной» программы_

Такие ошибки программы называются «утечки памяти». Они возникают, когда:

-   программист не удалил объект после использования;
-   указателю, ссылающемуся на объект в куче, присвоили новое значение, и других указателей на этот объект нет. Объект становится недоступным из кода, хотя продолжает занимать память.

В итоге приложение исчерпает доступный лимит памяти и завершится с ошибкой. Поэтому вовремя удаляйте ненужные объекты. Особенно в программах, которые должны работать долго и бесперебойно.

Чтобы исправить программу, добавьте вызов  `delete`  у указателя на строку в динамической памяти:

```cpp
#include <iostream>
#include <new>
#include <string>

using namespace std;

int main() {
    size_t n = 0;
    try {
        for (; n != 300; ++n) {
            string* p = new string(100'000'000, ' ');
            // Утечка памяти исправлена - объект в куче своевременно удаляется
            delete p;
        }
        cout << "Program completed successfully"s;
    } catch (const bad_alloc&) {
        // Сюда программа, скорее всего, не попадёт,
        // если объём свободной памяти в куче будет больше 100 мегабайт
        cout << "bad_alloc after "s << n << " allocations"s << endl;
    }
}

```

Запустите программу и убедитесь, что потребление памяти пришло в норму. Теперь программа использует немногим более сотни мегабайт памяти для хранения символов строки.

----------

Вы ведущий программист в команде, разрабатывающей софт для роботов-осьминогов. Один из джуниор-разработчиков прислал вам на ревью код, моделирующий осьминога:

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        for (int i = 1; i <= 8; ++i) {
            tentacles_.push_back(new Tentacle(i));
        }
    }

private:
    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

Взглянув на программу, вы сразу обнаруживаете в ней несколько утечек памяти и отправляете код на доработку. Какое исправление должен сделать программист, чтобы устранить найденные вами проблемы? Выберите один верный ответ. Предлагаемые исправления заключены между комментариями:

`// <---`

и

`// --->`

-   **Вариант 1**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
        }
        // --->
    }

    //<--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

-   **Вариант 2**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

-   **Вариант 3**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        try {
            for (int i = 1; i <= 8; ++i) {
                tentacles_.push_back(new Tentacle(i));
            }
        } catch (const bad_alloc&) {
            Cleanup();
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

-   **Вариант 4**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

-   **Вариант 5**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        for (int i = 1; i <= 8; ++i) {
            tentacles_.push_back(new Tentacle(i));
        }
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
    }
    // --->

private:
    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

----------

В программах на C++ применяют объекты с разным временем жизни. Долгоживущие объекты создаются в статической памяти и существуют на протяжении всей работы программы. Локальные переменные с автоматическим временем жизни наиболее просты в использовании — за их своевременным удалением следит компилятор.

Программа может обрабатывать произвольные объёмы данных, при необходимости создавая и удаляя объекты в динамической памяти. Чтобы работать с ними напрямую, нужно хорошо понимать возможные пути выполнения программы, в том числе при выбрасывании исключений.

Как правило, код, который использует низкоуровневые операции с динамической памятью, располагают внутри библиотечных классов и функций. Это позволяет при решении прикладных задач концентрироваться на бизнес-логике, а не на ручном управлении динамической памятью.

В следующих уроках вы научитесь созданию простых в использовании классов, скрывающих трудности работы с динамической памятью.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

----------

Вы ведущий программист в команде, разрабатывающей софт для роботов-осьминогов. Один из джуниор-разработчиков прислал вам на ревью код, моделирующий осьминога:

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        for (int i = 1; i <= 8; ++i) {
            tentacles_.push_back(new Tentacle(i));
        }
    }

private:
    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

Взглянув на программу, вы сразу обнаруживаете в ней несколько утечек памяти и отправляете код на доработку. Какое исправление должен сделать программист, чтобы устранить найденные вами проблемы? Выберите один верный ответ. Предлагаемые исправления заключены между комментариями:

`// <---`

и

`// --->`

-   **(-)**  **Вариант 1**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
        }
        // --->
    }

    //<--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

> Почти верно, но нет. Если в процессе конструирования щупалец будет выброшено исключение, обработчик  `catch`  их все удалит. Так как пойманное в конструкторе исключение не будет выброшено дальше, конструктор завершится успешно, а клиенту достанется осьминог без конечностей.

-   **(+)**  **Вариант 2**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

> Великолепно! В деструкторе класса  `Octopus`  вызов метода  `Cleanup`  удаляет созданные в динамической памяти щупальца. В конструкторе  `Octopus`  исключение  `bad_alloc`  может быть выброшено как в операции  `new`, так и в методе  `push_back`  класса  `vector`. Обработчик исключения учитывает обе ситуации, удаляя сконструированные щупальца, а потом пробрасывает исключение дальше, чтобы отменить конструирование осьминога.

-   **(-)**  **Вариант 3**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        try {
            for (int i = 1; i <= 8; ++i) {
                tentacles_.push_back(new Tentacle(i));
            }
        } catch (const bad_alloc&) {
            Cleanup();
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод cleanup
    void Cleanup() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

> Близко, но нет. Удаление щупалец в деструкторе осьминога реализовано верно. Обработка исключения, которое операция  `new`  может выбросить при создании щупальца, тоже правильная. Если в конструкторе метод  `push_back`  выбросит исключение, то указатель, возвращённый операцией  `new`, не будет помещён в контейнер  `tentacles_`  и не будет удалён в блоке  `catch`. Произойдёт утечка памяти.

-   **(-)**  **Вариант 4**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        // <--- Тело конструктора обновлено
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);
                tentacles_.push_back(t);
                t = nullptr;
            }
        } catch (const bad_alloc&) {
            Cleanup();
            delete t;
            throw bad_alloc();
        }
        // --->
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        Cleanup();
    }
    // --->

private:
    // <--- Добавлен метод Cleanup
    void Cleanup() {
        tentacles_.clear();
    }
    // --->

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

> В методе  `Cleanup`  очищается только контейнер указателей, а сами щупальца так и продолжат занимать память в куче. Для исправления проблемы надо сначала выполнить  `delete`  с каждым указателем в контейнере  `tentacles_`.

-   **(-)**  **Вариант 5**

```cpp
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// Осьминог
class Octopus {
public:
    Octopus() {
        for (int i = 1; i <= 8; ++i) {
            tentacles_.push_back(new Tentacle(i));
        }
    }

    // <--- Добавлен деструктор
    ~Octopus() {
        for (Tentacle* t : tentacles_) {
            delete t;
        }
    }
    // --->

private:
    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus;
}

```

> Этот код удалит щупальца из динамической памяти при вызове деструктора осьминога. Однако деструктор вызван не будет, если во время работы конструктора осьминога операция  `new Tentacle`  либо метод  `tentacles_.push_back`  выбросят исключение. Итог — утечка памяти, которую занимают сконструированные щупальца.

</details>

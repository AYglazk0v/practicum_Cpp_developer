<details>
<summary>Теория + тестирование:</summary>

# Независимость заголовочных файлов

Вы разобрались с тем, как разделять код по файлам: что добавить в заголовочный, а что — в файл с реализацией. Но иногда для класса нужен  `#include`  другого класса. Можно добавить его в заголовочный файл, а можно в реализацию. В этом уроке рассмотрим принципы, которыми надо руководствоваться при выборе места для  `#include`.

### .cpp vs .h

Проведём эксперимент и изменим включение  `#include "square_calculation.h"`. Почему-то мы решили включить его не в заголовочный файл carpenter.h, а в carpenter.cpp. Не будем ничего принимать на веру и перенесём  `#include "square_calculation.h"`  в carpenter.h.

carpenter.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Carpenter {
public:
    int CalcShelves(const Wall& wall) const;
};

```

carpenter.cpp

```cpp
#include "carpenter.h"

#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) / 2;
}

```

builder.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Builder {
public:
    double CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }
};

```

main.cpp

```cpp
#include "builder.h"
#include "carpenter.h"
#include "painter.h"

#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Carpenter jack;
    Wall wall(3.5, 2.45);

    cout << tom.CalcBricksNeeded(wall) << endl;
    cout << bill.CalcPaintNeeded(wall) << endl;
    cout << jack.CalcShelves(wall) << endl;
    return 0;
}

```

painter.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Painter {
public:
    void Paint(Wall& wall, Wall::Color color) const {
        wall.SetColor(color);
    }
    double CalcPaintNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 0.4;
    }
};

```

square_calculation.cpp

```cpp
#include "square_calculation.h"

double CalcSquare(double width, double height) {
    return width * height;
}

```

square_calculation.h

```cpp
#pragma once
double CalcSquare(double width, double height);

```

wall.h

```cpp
#pragma once

class Wall {
public:
    enum class Color { BLUE, GREEN, RED, WHITE, YELLOW };

    Wall(double width, double height);

    double GetHeight() const;
    double GetWidth() const;
    void SetColor(Color color);
    Color GetColor() const;

private:
    double width_;
    double height_;
    Color color_;
};

```

wall.cpp

```cpp
#include "wall.h"

Wall::Wall(double width, double height)
    : width_(width)
    , height_(height)
    , color_(Color::WHITE) {
}

double Wall::GetHeight() const {
    return height_;
}
double Wall::GetWidth() const {
    return width_;
}
void Wall::SetColor(Color color) {
    color_ = color;
}
Color Wall::GetColor() const {
	return color_;
}

```

Запустим. It’s alive! Программа работает, как прежде, ведь carpenter.h включён в carpenter.cpp, а вместе с ним включены все его включения. Непонятно, в чём тогда разница. Сейчас увидите.

Вы уже учились думать как компилятор. Этот приём помог разобраться в сути ошибок компиляции. Здесь ошибки нет, но проверить, всё ли в порядке, нужно. Освоим ещё один ментальный приём — «думай как пользователь»‎. У кода, который вы создаёте, есть пользователь, иначе писать его не имело бы смысла. Здесь под пользователем имеется в виду не человек, а файл, который будет использовать ваш класс. Файл main.cpp — пользователь классов  `Builder`  и  `Painter`. А  `Builder`  в свою очередь — пользователь функции  `CalcSquare`.

Представим, что в классе много разных методов, и только один из них использует функцию из square_calculation.h. Пока ничего страшного. Посмотрим с точки зрения пользователя. Чтобы использовать метод класса  `Carpenter`, нужно добавить  `#include "carpenter.h"`. Вместе с ним включится и square_calculation.h. Но пользователю он совершенно не нужен. Он использует метод класса  `Carpenter`  и не собирается брать ничего из square_calculation.h. Во время препроцессинга  `#include "square_calculation.h"`  превратится в текст файла square_calculation.h, потом компилятор будет последовательно проверять каждую строчку. А пользователю код этого файла не нужен. Одним маленьким включением можно увеличивать размер кода в несколько раз. Следом увеличится время компиляции, и программист пойдёт пить пятую чашку кофе.

Простой вывод: включайте файлы только там, где они действительно используются. Не создавайте избыточных включений.

### Гордые и независимые

Другой важный принцип — независимость заголовочных файлов. Посмотрите, как изменился проект. Плотник научился устанавливать двери. Для этого ему нужна стена и помощник-строитель. Строитель должен подержать дверь, пока тот её установит. Так как теперь файл carpenter.h должен включать builder.h, можно убрать включение wall.h. Ведь builder.h уже включает wall.h. Лишние включения нам не нужны — это увеличивает объём кода, усложняет жизнь компилятору и заставляет программиста портить здоровье кофеином.

carpenter.h

```cpp
#pragma once
#include "builder.h"
#include "square_calculation.h"

class Carpenter {
public:
    int CalcShelves(const Wall& wall) const;
    void InstallDoor(Wall& wall, const Builder& builder) const;
};

```

carpenter.cpp

```cpp
#include "carpenter.h"

#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) / 2;
}

void Carpenter::InstallDoor(Wall& wall, const Builder& builder) const {
    // Hold the door, builder! Hold the door!
    builder.HoldDoor();
    wall.SetDoorInstalled();
}

```

builder.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Builder {
public:
    double CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }

    void HoldDoor() const {
        for (int i = 100; i != 0; --i) {
            /* держит дверь */
        }
    }
};

```

main.cpp

```cpp
#include "builder.h"
#include "carpenter.h"
#include "painter.h"

#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Carpenter jack;
    Wall wall(3.5, 2.45);

    jack.InstallDoor(wall, tom);
    cout << wall.IsDoorInstalled() << endl;
    return 0;
}

```

painter.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Painter {
public:
    void Paint(Wall& wall, Wall::Color color) const {
        wall.SetColor(color);
    }
    double CalcPaintNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 0.4;
    }
};

```

square_calculation.cpp

```cpp
#include "square_calculation.h"

double CalcSquare(double width, double height) {
    return width * height;
}

```

square_calculation.h

```cpp
#pragma once
double CalcSquare(double width, double height);

```

wall.cpp

```cpp
#include "wall.h"

Wall::Wall(double width, double height)
    : width_(width)
    , height_(height)
    , color_(Color::WHITE)
    , is_door_installed_(false) {
}

double Wall::GetHeight() const {
    return height_;
}
double Wall::GetWidth() const {
    return width_;
}
void Wall::SetColor(Color color) {
    color_ = color;
}
Color Wall::GetColor() const {
	return color_;
}
bool Wall::IsDoorInstalled() const {
    return is_door_installed_;
}

void Wall::SetDoorInstalled() {
    is_door_installed_ = true;
}

```

wall.h

```cpp
#pragma once

class Wall {
public:
    enum class Color { BLUE, GREEN, RED, WHITE, YELLOW };

    Wall(double width, double height);

    double GetHeight() const;
    double GetWidth() const;
    void SetColor(Color color);
    Color GetColor() const;
    bool IsDoorInstalled() const;
    void SetDoorInstalled();

private:
    double width_;
    double height_;
    Color color_;
    bool is_door_installed_;
};

```

Проект работает, дверь установлена. Казалось бы, что может пойти не так. Предположим, однажды рабочий и маляр откажутся считать кирпичи и количество краски и передадут эту задачу новому классу  `Accountant`. Тогда  `#include "wall.h"`  станет классу  `Builder`  совсем не нужен.

builder.h

```cpp
#pragma once
#include "square_calculation.h"

class Builder {
public:
    void HoldDoor() const {
        for (int i = 100; i != 0; --i) {
            /* держит дверь */
        }
    }
};

```

accountant.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Accountant {
public:
    double CalcPaintNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 0.4;
    }

    double CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }
};

```

carpenter.cpp

```cpp
#include "carpenter.h"

#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) / 2;
}

void Carpenter::InstallDoor(Wall& wall, const Builder& builder) const {
    // Hold the door, builder! Hold the door!
    builder.HoldDoor();
    wall.SetDoorInstalled();
}

```

carpenter.h

```cpp
#pragma once
#include "builder.h"

class Carpenter {
public:
    int CalcShelves(const Wall& wall) const;
    void InstallDoor(Wall& wall, const Builder& builder) const;
};

```

main.cpp

```cpp
#include "accountant.h"
#include "builder.h"
#include "carpenter.h"
#include "painter.h"

#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Carpenter jack;
    Accountant ray;
    Wall wall(3.5, 2.45);

    cout << ray.CalcBricksNeeded(wall) << endl;
    cout << ray.CalcPaintNeeded(wall) << endl;

    jack.InstallDoor(wall, tom);
    cout << wall.IsDoorInstalled() << endl;
    return 0;
}

```

painter.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

class Painter {
public:
    void Paint(Wall& wall, Wall::Color color) const {
        wall.SetColor(color);
    }
};

```

square_calculation.cpp

```cpp
#include "square_calculation.h"

double CalcSquare(double width, double height) {
    return width * height;
}

```

square_calculation.h

```cpp
#pragma once
double CalcSquare(double width, double height);

```

wall.cpp

```cpp
#include "wall.h"

Wall::Wall(double width, double height)
    : width_(width)
    , height_(height)
    , color_(Color::WHITE)
    , is_door_installed_(false) {
}

double Wall::GetHeight() const {
    return height_;
}
double Wall::GetWidth() const {
    return width_;
}
void Wall::SetColor(Color color) {
    color_ = color;
}
Color Wall::GetColor() const {
	return color_;
}
bool Wall::IsDoorInstalled() const {
    return is_door_installed_;
}

void Wall::SetDoorInstalled() {
    is_door_installed_ = true;
}

```

wall.h

```cpp
#pragma once

class Wall {
public:
    enum class Color { BLUE, GREEN, RED, WHITE, YELLOW };

    Wall(double width, double height);

    double GetHeight() const;
    double GetWidth() const;
    void SetColor(Color color);
    Color GetColor() const;
    bool IsDoorInstalled() const;
    void SetDoorInstalled();

private:
    double width_;
    double height_;
    Color color_;
    bool is_door_installed_;
};

```

----------

Какая ошибка возникает при компиляции этого проекта?

-   Ошибка препроцессинга (первого этапа компиляции)
    
-   Ошибка компиляции (второго этапа компиляции)
    
-   Ошибка компоновки (третьего этапа компиляции)Это ошибка компиляции (второго этапа). При попытке проверить код в файле carpenter.cpp компилятор не понимает, что такое  `Wall`. Соответствующего подключения заголовочного файла нет, потому что раньше carpenter.cpp получал его от builder.h, а теперь строитель не имеет дела со стенами. Выходит, при изменении файла builder.h нужно вносить изменение в carpenter.h. А если бы таких зависимых файлов было много, страшно представить, сколько изменений пришлось бы вносить программисту, и как много ошибок он наделал бы по дороге.
    

Отсюда принцип независимости заголовочных файлов: каждый заголовочный файл должен включать в себя всё необходимое для своей работы. Так мы исключим ситуацию, когда проект меняется в одном месте, а ломается в другом. Чем более инкапсулированы файлы, тем более гибким для изменений станет проект.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

----------

Какая ошибка возникает при компиляции этого проекта?

-   **(-)**  Ошибка препроцессинга (первого этапа компиляции)
    
-   **(+)**  Ошибка компиляции (второго этапа компиляции)
    
-   **(-)**  Ошибка компоновки (третьего этапа компиляции)

</details>

<details>
<summary>Задание 1:</summary>

## Задание 1

Почините проект так, чтобы заголовочные файлы стали независимыми.

### Подсказка

Не хватает подключения всего одного заголовочного файла.

</details>

<details>
<summary>Задание 2:</summary>

## Задание 2

Кое-что в файле accountant.h можно выделить в файл с реализацией accountant.cpp. Перенесите определения методов класса в новый файл.

### Подсказка

Проверьте, где действительно используется функция из square_calculation.h и включён ли этот файл там, где нужно.

</details>

<details>
<summary>Теория:</summary>

# Заголовочные файлы и файлы с реализацией

По аналогии с подсчётом количества кирпичей научим вашего маляра считать расход краски:

```cpp
#include "wall.h"

class Painter {
public:
    void Paint(Wall& wall, Wall::Color color) const {
        wall.SetColor(color);
    }
    double CalcPaintNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        // Пусть расход краски считается вот так
		    return width * height * 0.4;
    }
};

```

Но подождите. Выглядит знакомо. И расход краски, и количество кирпичей зависят от площади стены. Сделаем новый файл с функцией, которая будет считать площадь по известной ширине и высоте.

square_calculation.h

```cpp
double CalcSquare(double width, double height) {
    return width * height;
}

```

Применим функцию  `CalcSquare`  для подсчёта кирпичей и краски.

builder.h

```cpp
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

painter.h

```cpp
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

main.cpp

```cpp
#include "builder.h"
#include "painter.h"
#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Wall wall(3.5, 2.45);

    cout << tom.CalcBricksNeeded(wall) << endl;
    cout << bill.CalcPaintNeeded(wall) << endl;
}

```

Компилируем и видим многообразие ошибок:

```
In file included from painter.h:1:0,
                 from main.cpp:2:
square_calculation.h: In function 'double CalcSquare(double, double)':
square_calculation.h:1:8: error: redefinition of 'double CalcSquare(double, double)'
 double CalcSquare(double width, double height) {
        ^~~~~~~~~~
In file included from builder.h:1:0,
                 from main.cpp:1:
square_calculation.h:1:8: note: 'double CalcSquare(double, double)' previously defined here
 double CalcSquare(double width, double height) {
        ^~~~~~~~~~
In file included from painter.h:2:0,
                 from main.cpp:2:
wall.h: At global scope:
wall.h:1:7: error: redefinition of 'class Wall'
 class Wall {
       ^~~~
In file included from builder.h:2:0,
                 from main.cpp:1:
wall.h:1:7: error: previous definition of 'class Wall'
 class Wall {
       ^~~~

```

Посмотрим на первую из них: “redefinition of double CalcSquare(double, double)”. Сообщение об ошибке говорит, что у функции CalcSquare несколько определений. Разберёмся, как так вышло.

Вряд ли вам доводилось думать как компилятор. Сейчас самое время. Чтобы понять, что компилятор делает с файлами, пойдём по коду строчка за строчкой. Заменим директивy  `#include "square_calculation.h"`  на соответствующий файл. Получим:

builder.h

```cpp
double CalcSquare(double width, double height) {
    return width * height;
}

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

painter.h

```cpp
double CalcSquare(double width, double height) {
    return width * height;
}

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

А теперь заменим  `#include "builder.h"`  и  `#include "painter.h"`  в main.cpp:

```cpp
double CalcSquare(double width, double height) {
    return width * height;
}

#include "wall.h"

class Builder {
public:
    double CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }
};

double CalcSquare(double width, double height) {
    return width * height;
}

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

#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Wall wall(3.5, 2.45);

    cout << tom.CalcBricksNeeded(wall) << endl;
    cout << bill.CalcPaintNeeded(wall) << endl;
}

```

Получилось, что в одном файле две функции  `CalcSquare`, и обе определены — имеют тело. Компилятор не сравнивает, одинаковая ли у этих функций реализация. Он просто видит, что их две, и знает: у каждой функции может быть только одно тело.

А теперь к самому интересному — исправлению ошибки. Если вы внимательны, наверняка уже заметили: файл  `main`  имеет расширение .cpp, а добавленные файлы — расширение .h. И те, и другие написаны на языке С++, и глобальной разницы между ними нет.

Файлы с расширением .h или реже .hpp называются заголовочными, а файлы с расширением .cpp — файлами с реализацией. Заголовочные файлы содержат объявления функций, а файлы с реализацией — определения функций.

Оставим определение функции в square_calculation.h и добавим файл square_calculation.cpp.

square_calculation.h

```cpp
double CalcSquare(double width, double height);

```

square_calculation.cpp

```cpp
double CalcSquare(double height, double width) {
    return width * height;
}

```

После замены  `#include`  “builder.h” и  `#include`  “painter.h” main.cpp будет выглядеть так:

```cpp
double CalcSquare(double width, double height);

#include "wall.h"

class Builder {
public:
    double CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }
};

double CalcSquare(double width, double height);

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

#include <iostream>

using namespace std;

int main() {
    Builder tom;
    Painter bill;
    Wall wall(3.5, 2.45);

    cout << tom.CalcBricksNeeded(wall) << endl;
    cout << bill.CalcPaintNeeded(wall) << endl;
}

```

Больше проблемы с двумя определениями функции  `CalcSquare`  нет.

В мире больших реальных проектов практически не бывает заголовочных файлов с одной функцией. Обычно в них гораздо больше информации. Поэтому принято включать заголовочный файл в соответствующий файл с реализацией и называть их одинаково, но с разными расширениями. То есть в финальном варианте файл square_calculation.cpp будет выглядеть так:

```cpp
#include "square_calculation.h"

double CalcSquare(double width, double height) {
    return width * height;
}
```

</details>

<details>
<summary>Задание:</summary>

## Задание 1

Выделите в отдельный файл wall.cpp определения методов класса  `Wall`.

**Пример**

```
double w = 3.5;
  double h = 2.45;
  Wall wall(w, h);
  // wall.GetWidth() == w
  // wall.GetHeight() == h

  wall.SetColor(Wall::Color::BLUE);

  // wall.GetColor() == Wall::Color::BLUE
  // wall.GetWidth() == w
  // wall.GetHeight() == h

```

### Подсказка

Ещё один пример определения метода класса  `Wall`:

```cpp
double Wall::GetHeight() const {
    return height_;
}
```

</details>

<details>
<summary>Задание:</summary>

## Задание 2

Строитель Том построил стену. Маляр Билл покрасил её в ваш любимый цвет. Теперь нужен столяр Джек. Он сможет сказать, сколько на эту стену поместится красивых полок для вашей коллекции музыкальных шкатулок. Создайте новый класс  `Carpenter`. Пусть у него будет метод класса, считающий количество полок исходя из того, что на два квадратных метра стены можно повесить одну полку. Назовите этот метод  `CalcShelves`. В качестве параметра он должен получать объект класса  `Wall`.

**Пример**

```
Carpenter jack;
Wall wall(3.5, 2.45);
// jack.CalcShelves(wall) == 4

```

### Подсказка

Количество полок — целое число, а файл square_calculation.h должен быть включён всего один раз.

</details>

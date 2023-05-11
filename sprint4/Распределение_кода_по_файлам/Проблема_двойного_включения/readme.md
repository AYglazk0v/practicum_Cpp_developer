<details>
<summary>Теория:</summary>

# Проблема двойного включения

Вернёмся к строительству дома. В его проекте девять файлов.

wall.h

```cpp
class Wall {
public:
    enum class Color {
        BLUE,
        GREEN,
        RED,
        WHITE,
        YELLOW
    };

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
    : width_(width), height_(height), color_(Color::WHITE) 
{
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

square_calculation.h

```cpp
double CalcSquare(double width, double height);

```

square_calculation.cpp

```cpp
#include "square_calculation.h"

double CalcSquare(double width, double height) {
	return width * height;
}

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
#include "carpenter.h"
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

carpenter.h

```cpp
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
#include "square_calculation.h"
#include "wall.h"

class Builder {
public:
    double  CalcBricksNeeded(const Wall& wall) const {
        double height = wall.GetHeight();
        double width = wall.GetWidth();
        return CalcSquare(width, height) * 5;
    }
};

```

Проект не компилируется, выскакивает ошибка “Wall: class type redefinition”. В уроке «Заголовочные файлы и файлы с реализацией» из-за двойного включения одной и той же функции возникала ошибка с дублированием определения функции  `CalcSquare`. Определение функции вы убрали в файл с реализацией, а в заголовочном файле оставили только объявление. При создании объектного файла ошибок не возникало, потому что компилятор знал, что это за функция. Во время сборки в единый исполняемый файл компоновщик находил эту функцию как «‎экспортируемую»‎ модулем square_calculations.o. Внимательно посмотрим, что получалось после замены  `#include "builder.h"`,  `#include "painter.h"`  и  `#include "carpenter.h"`, и увидим, что  `#include "wall.h"`  включён трижды:

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

#include "wall.h"

class Carpenter {
public:
    int CalcShelves(const Wall& wall) const;
};

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
}

```

Теперь понятно, что не нравится компилятору. Определение класса  `Wall`  встречается несколько раз. Но убрать определение класса в файл с реализацией нельзя. Ведь нужно знать, какие методы есть у класса, чтобы их вызывать.

Для подобных ситуаций есть другая директива —  `#pragma once`. Судя по # в начале, это директива препроцессора. Значит, обработана она будет на первом этапе компиляции.  `#pragma once`  оберегает от двойного включения одного и того же заголовочного файла. Если файл уже был явно или, как в нашем случае, неявно включён в файл, второй раз включаться он уже не будет. Нет повторного определения классов — меньше кода. Сплошной profit.

Добавляйте  `#pragma once`  во все свои заголовочные файлы. Так вы избежите ошибок и не будете тратить время, пытаясь разобраться, что не так. Делать это руками необязательно. В IDE вы можете создать шаблон для заголовочных файлов. Это не раз пригодится вам в работе.

</details>

<details>
<summary>Задание:</summary>

## Задание

Добавьте во все заголовочные файлы проекта директиву  `#pragma once`.

### Подсказка

Заголовочных файлов у вас пять. Не забудьте добавить  `#pragma once`  в каждый.

</details>

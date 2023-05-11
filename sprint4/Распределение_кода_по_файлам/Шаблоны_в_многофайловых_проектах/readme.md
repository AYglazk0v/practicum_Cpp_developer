
<details>
<summary>Теория + тестирование:</summary>

# Шаблоны в многофайловых проектах

Рабочие успешно справляются со строительством стен. Но кроме стен у дома обычно бывает пол и крыша. У них тоже есть площадь, поэтому нужно посчитать количество краски и кирпичей и для них.

Создадим в проекте классы  `Roof`  и  `Ceiling`. Чтобы у всех строительных объектов —  `Roof`,  `Ceiling`,  `Wall`  — был метод с одинаковым названием и типом возвращаемого значения, добавим в класс  `Wall`  метод  `GetSizes`.

Кроме того, нужно уметь делать расчёты для любых объектов, имеющих метод  `GetSizes`, который возвращает пару значений  `double`. Сделать это можно, применив шаблоны:

accountant.cpp

```cpp
#include "accountant.h"

#include "square_calculation.h"

using namespace std;

template <class BuildingObject>
double Accountant::CalcPaintNeeded(const BuildingObject& building_object) const {
    pair<double, double> sizes = building_object.GetSizes();
    return CalcSquare(sizes.first, sizes.second) * 0.4;
}

template <class BuildingObject>
double Accountant::CalcBricksNeeded(const BuildingObject& building_object) const {
    pair<double, double> sizes = building_object.GetSizes();
    return CalcSquare(sizes.first, sizes.second) * 5;
}

```

accountant.h

```cpp
#pragma once
#include "square_calculation.h"
#include "wall.h"

#include <utility>

class Accountant {
public:
    template <class BuildingObject>
    double CalcPaintNeeded(const BuildingObject& building_object) const;
    template <class BuildingObject>
    double CalcBricksNeeded(const BuildingObject& building_object) const;
};

```

builder.h

```cpp
#pragma once
#include "square_calculation.h"

class Builder {
public:
    void HoldDoor() const {
        // Просто держит дверь 100 условных секунд
        int i = 100;
        while (0 != i) {
            i--;
        }
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
#include "wall.h"

class Carpenter {
public:
    int CalcShelves(const Wall& wall) const;
    void InstallDoor(Wall& wall, const Builder& builder) const;
};

```

ceiling.cpp

```cpp
#include "ceiling.h"

Ceiling::Ceiling(double length, double width)
    : length_(length)
    , width_(width) {
}

std::pair<double, double> Ceiling::GetSizes() const {
    return {length_, width_};
}

```

ceiling.h

```cpp
#pragma once
#include <utility>

class Ceiling {
public:
    Ceiling(double length, double width);
    std::pair<double, double> GetSizes() const;

private:
    double length_;
    double width_;
};

```

main.cpp

```cpp
#include "accountant.h"
#include "ceiling.h"
#include "roof.h"

#include <iostream>

using namespace std;

int main() {
    Accountant ray;
    Wall wall(3.5, 2.45);
    Roof roof(5, 7);
    Ceiling ceiling(5, 7);

    cout << "Требуется кирпичей: "s
         << ray.CalcBricksNeeded<Wall>(wall) + ray.CalcBricksNeeded<Roof>(roof) + ray.CalcBricksNeeded<Ceiling>(ceiling)
         << endl;

    cout << "Требуется краски: "s
         << ray.CalcPaintNeeded<Wall>(wall) + ray.CalcPaintNeeded<Roof>(roof) + ray.CalcPaintNeeded<Ceiling>(ceiling)
         << endl;
		return 0;
}

```

painter.h

```jsx
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

roof.cpp

```jsx
#include "roof.h"

Roof::Roof(double length, double width)
    : length_(length)
    , width_(width) {
}

std::pair<double, double> Roof::GetSizes() const {
    return {length_, width_};
}

```

roof.h

```cpp
#pragma once
#include <utility>

class Roof {
public:
    Roof(double length, double width);
    std::pair<double, double> GetSizes() const;

private:
    double length_;
    double width_;
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
std::pair<double, double> Wall::GetSizes() const {
    return {width_, height_};
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
#include <utility>

class Wall {
public:
    enum class Color { BLUE, GREEN, RED, WHITE, YELLOW };

    Wall(double width, double height);

    double GetHeight() const;
    double GetWidth() const;
    std::pair<double, double> GetSizes() const;
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

Запускаем. Что-то пошло не так:

```
undefined reference to double Accountant::CalcBricksNeeded<Ceiling>(Ceiling const&) const' 
undefined reference to double Accountant::CalcBricksNeeded<Roof>(Roof const&) const'
undefined reference to double Accountant::CalcBricksNeeded<Wall>(Wall const&) const' 
undefined reference to double Accountant::CalcPaintNeeded<Ceiling>(Ceiling const&) const'
undefined reference to double Accountant::CalcPaintNeeded<Roof>(Roof const&) const' 
undefined reference to double Accountant::CalcPaintNeeded<Wall>(Wall const&) const'

```

----------

Эти ошибки возникли при работе:

-   Препроцессора
    
-   Компилятора
    
-   КомпоновщикаКомпоновщик не смог найти в коде перечисленные функции. Подозрительно, ведь мы всё сделали правильно. Объявление оставили в заголовочном файле, а определение — в файле с реализацией. До того как добавили шаблоны, код работал. Дело в том, что шаблоны действуют не так, как обычные функции, — компилятору приходится самому разбираться, какие функции сделать из этого шаблона.
    

Когда нажимаем кнопку начала компиляции, первым в городе просыпается препроцессор. Он заменяет все свои директивы и ложится спать.

Просыпается компилятор. Когда он приходит разбираться с объектом main.cpp, видит объявление шаблона из accountant.h, вставленное препроцессором. Компилятор, читая строчку за строчкой, доходит до использования шаблонов с типами  `Wall`,  `Roof`  и  `Ceiling`. Создаёт из шаблонов объявлений (не определений!) соответствующее объявление функции и идёт дальше. До конца файла main.cpp он доходит с надеждой, что компоновщик разберётся, где находятся функции  `CalcBricksNeeded`  и  `CalcPaintNeeded`, имеющие пока только объявление.

Каждый файл компилируется отдельно. Поэтому когда компилятор работает над файлом accountant.cpp, он уже не помнит, что шаблонами пытались воспользоваться. Он только видит, что в текущем файле никто эти шаблоны не применяет, и никаких функций не создаётся. Компилятор заканчивает и засыпает.

Просыпается компоновщик. Он усердно ищет каждому импорту свой экспорт. Компоновщик ничего не знает о шаблонах. Он видит, что для main.cpp нужны функции  `Accountant::CalcBricksNeeded<Ceiling>(Ceiling const&)`,  `Accountant::CalcBricksNeeded<Roof>(Roof const&)`  и  `Accountant::CalcBricksNeeded<Wall>(Wall const&) const'`. Ни у какого файла такого экспорта нет. Компоновщик выставляет ошибки и заканчивает работу. Проект не собрался. Время мирным программистам разобраться.

</details>

<details>
<summary>Ответы:</summary>

# Ответы на задания

Эти ошибки возникли при работе:

-   **(-)**  Препроцессора
    
-   **(-)**  Компилятора
    
-   **(+)**  Компоновщика

</details>

<details>
<summary>Задание:</summary>

## Задание

Исправьте проект так, чтобы шаблоны заработали.

### Подсказка

Возможно, какой-либо файл с реализацией вам больше не нужен.

</details>

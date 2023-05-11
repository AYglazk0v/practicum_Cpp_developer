#include "painter.h"
#include <iostream>

int main() {
    Painter bill;
    Wall wall(3.5, 2.45);

    std::cout << "BEFORE COLOR: " << static_cast<int>(wall.GetColor()) << std::endl;
    bill.Paint(wall, Wall::Color::BLUE);
    std::cout << "AFTER COLOR: " << static_cast<int>(wall.GetColor()) << std::endl;
}
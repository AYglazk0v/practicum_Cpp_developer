#include <iostream>
#include "carpenter.h"

int main() {
    Carpenter jack;
    Wall wall{3.5, 2.45};
    std::cout << jack.CalcShelves(wall);
    return 0;
}
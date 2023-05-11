#include <iostream>
#include "builder.h"

int main() {
    Builder tom;
    std::cout << tom.CalcBricksNeeded({3.5, 2.45});
    // Попросите Тома посчитать кирпичи для стены 3.5 х 2.45
}
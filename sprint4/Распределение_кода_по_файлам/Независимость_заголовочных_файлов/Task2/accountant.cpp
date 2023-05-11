#include "accountant.h"
#include "square_calculation.h"

double Accountant::CalcBricksNeeded(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) * 5;
}

double Accountant::CalcPaintNeeded(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) * 0.4;
}
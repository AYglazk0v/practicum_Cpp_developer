#include "carpenter.h"

int Carpenter::CalcShelves(const Wall& wall) const {
	return (CalcSquare(wall.GetHeight(), wall.GetWidth()) / 2);
}
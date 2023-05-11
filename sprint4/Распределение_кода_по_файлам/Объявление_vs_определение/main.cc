#include <iostream>

using namespace std;

void Egg(int);

void Hen() {
    cout << "Курица вызывает яйцо"s << endl;
    Egg(0);
}

void Egg(int x) {
    if (1 == x) {
        cout << "Яйцо вызывает курицу"s << endl;
        Hen();
    } else {
        cout << "А вот и цыплёнок!"s << endl;
    }
}

int main() {
    Egg(1);
}
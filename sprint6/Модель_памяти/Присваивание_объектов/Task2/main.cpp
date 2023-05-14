#include "octopus.h"

#include <cassert>
#include <iostream>

using namespace std;

int main() {
    // Проверка присваивания осьминогов
    {
        Octopus octopus1(3);

        // Настраиваем состояние исходного осьминога
        octopus1.GetTentacle(2).LinkTo(octopus1.GetTentacle(1));

        // До присваивания octopus2 имеет своё собственное состояние
        Octopus octopus2(10);

        octopus2 = octopus1;

        // После присваивания осьминогов щупальца копии имеют то же состояние,
        // что и щупальца присваиваемого объекта
        assert(octopus2.GetTentacleCount() == octopus1.GetTentacleCount());
        for (int i = 0; i < octopus2.GetTentacleCount(); ++i) {
            auto& tentacle1 = octopus1.GetTentacle(i);
            auto& tentacle2 = octopus2.GetTentacle(i);
            assert(&tentacle2 != &tentacle1);
            assert(tentacle2.GetId() == tentacle1.GetId());
            assert(tentacle2.GetLinkedTentacle() == tentacle1.GetLinkedTentacle());
        }
    }

    // Проверка самоприсваивания осьминогов
    {
        Octopus octopus(3);

        // Настраиваем состояние осьминога
        octopus.GetTentacle(0).LinkTo(octopus.GetTentacle(1));

        vector<pair<Tentacle*, Tentacle*>> tentacles;
        // Сохраняем информацию о щупальцах осьминога и его копии
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            tentacles.push_back({&octopus.GetTentacle(i), octopus.GetTentacle(i).GetLinkedTentacle()});
        }

        // Выполняем самоприсваивание
        octopus = octopus;

        // После самоприсваивания состояние осьминога не должно измениться
        assert(octopus.GetTentacleCount() == static_cast<int>(tentacles.size()));
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            auto& tentacle_with_link = tentacles.at(i);
            assert(&octopus.GetTentacle(i) == tentacle_with_link.first);
            assert(octopus.GetTentacle(i).GetLinkedTentacle() == tentacle_with_link.second);
        }
    }
}
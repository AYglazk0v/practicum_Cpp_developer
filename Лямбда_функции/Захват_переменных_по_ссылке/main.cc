#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
    const map<string, int>& max_amount, map<string, int>& shelter) {
    return std::count_if(new_dogs.begin(), new_dogs.end(), [&shelter, &max_amount](const std::string& curr_dog){
        if (max_amount.contains(curr_dog) && max_amount.at(curr_dog) > shelter[curr_dog]) {
            shelter[curr_dog]++;
            return true;
        }
        return false;
    });
}

int main() {
    map<string, int> shelter {
    {"landseer"s, 1},
    {"otterhound"s, 2},
    {"pekingese"s, 2},
    {"pointer"s, 3}
    };

    const map<string, int> max_amount {
    {"landseer"s, 2},
    {"otterhound"s, 3},
    {"pekingese"s, 4},
    {"pointer"s, 7}
    };

    const vector<string> new_dogs {
    "landseer"s,
    "otterhound"s,
    "otterhound"s,
    "otterhound"s,
    "pointer"s,
    "asdg"s
    };

    std::cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << std::endl;
    return 0;
}
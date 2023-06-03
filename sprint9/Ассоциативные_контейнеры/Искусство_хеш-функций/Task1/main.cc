#include <iostream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    int count_collisions = 0;
    std::string word;
    std::unordered_map<uint64_t, std::unordered_set<std::string>> u_map;

    while (text >> word) {
        uint64_t tmp = hasher(word);
        count_collisions += u_map.contains(tmp) && !u_map[tmp].contains(word) ? 1 : 0;
        u_map[tmp].insert(std::move(word));
    }
    return count_collisions;
}

struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}
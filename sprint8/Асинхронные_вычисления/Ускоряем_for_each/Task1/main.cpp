#include <algorithm>
#include <execution>
#include <thread>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <future>

#include "log_duration.h"

using namespace std;

int count_threads = [](){
    return std::thread::hardware_concurrency();
}();

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

template <template <typename> typename Container>
Container<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, max_length));
    }
    return Container(words.begin(), words.end());
}

struct Reverser {
    void operator()(string& value) const {
        reverse(value.begin(), value.end());
    }
};

template <typename Container, typename Function>
void Test(string_view mark, Container keys, Function function) {
    LOG_DURATION(mark);
    function(keys, Reverser{});
}

#define TEST(function) Test(#function, keys, function<remove_const_t<decltype(keys)>, Reverser>)

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    auto count_next = range.size() / count_threads;
    auto it_s = range.begin();
    auto it_e = std::next(range.begin(), count_next);
    vector<future<void>> futures;
    for (int i = 0; i != count_threads; ++i) {
        futures.push_back(std::async([&it_s, &it_e, &function]{
            for_each(it_s, it_e, function);
        }));
        it_s = it_e;
        if (i == count_threads - 1) {
            it_e = range.end();
        }
        else {
            it_e = next(it_s, count_next);
        }
    }
}

int main() {
    // для итераторов с произвольным доступом тоже должно работать
    vector<string> strings = {"cat", "dog", "code"};

    ForEach(strings, [](string& s) {
        reverse(s.begin(), s.end());
    });

    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
    // вывод: tac god edoc

    mt19937 generator;
    const auto keys = GenerateDictionary<list>(generator, 50'000, 5'000);

    TEST(ForEach);

    return 0;
}
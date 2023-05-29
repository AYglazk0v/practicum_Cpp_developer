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

template<typename ExecutionPolicy, typename ForwardRange, typename Function>
void ForEach(ExecutionPolicy&& policy, ForwardRange& range, Function function) {
    if constexpr (std::is_same_v<std::decay_t<ExecutionPolicy>, std::execution::sequenced_policy>
                || is_same_v<typename std::iterator_traits<typename ForwardRange::iterator>::iterator_category, std::random_access_iterator_tag>) {
        std::for_each(policy, range.begin(), range.end(), function);
    } else {
        auto count_next = range.size() / count_threads;
        count_next = count_next > 0 ? count_next : 1;

        auto it_s = range.begin();
        auto it_e = range.begin();
        vector<future<void>> futures;
        for (int i = 0; it_e != range.end() ; ++i) {
            if (i == count_threads - 1) {
                it_e = range.end();
            }
            else {
                it_e = next(it_s, count_next);
            }
            futures.push_back(std::async([it_s, it_e, function]{
                for_each(it_s, it_e, function);
            }));
            it_s = it_e;
        }
    }
}

template<typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    ForEach(execution::seq, range, function);
}

template <typename Strings>
void PrintStrings(const Strings& strings) {
    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
}

int main() {
    auto reverser = [](string& s) {
        reverse(s.begin(), s.end());
        };


    list<string> strings_list = {"cat", "dog", "code"};

    ForEach(strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    ForEach(execution::seq, strings_list, reverser);
    PrintStrings(strings_list);
    // cat dog code

    // единственный из вызовов, где должна работать ваша версия
    // из предыдущего задания
    ForEach(execution::par, strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    vector<string> strings_vector = {"cat", "dog", "code"};

    ForEach(strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    ForEach(execution::seq, strings_vector, reverser);
    PrintStrings(strings_vector);
    // cat dog code

    ForEach(execution::par, strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    return 0;
}
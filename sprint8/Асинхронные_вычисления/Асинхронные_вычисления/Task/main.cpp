#include <algorithm>
#include <execution>
#include <iostream>
#include <future>
#include "log_duration.h"

using namespace std;

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::sequenced_policy&,
                                RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound) {
        const auto middle = left_bound + (right_bound - left_bound) / 2;
        if (*middle < value) {
            left_bound = middle;
        } else {
            right_bound = middle;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    } else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::parallel_policy&,
                                RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound) {
        const auto distance = std::distance(left_bound, right_bound);
        const auto section_length = std::max<int>(1, distance / 3);
        const auto middle_left = left_bound + section_length;
        const auto middle_right = right_bound - section_length;

        std::future<bool> midleft_to_midright = std::async([middle_left, &value] { return *middle_left < value; });

        if (*middle_right < value) {
            left_bound = middle_right;
        } else if (midleft_to_midright.get()) {
           left_bound = middle_left;
           right_bound = middle_right;
        } else
            {
            right_bound = middle_left;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    } else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    return LowerBound(execution::seq, range_begin, range_end, value);
}

int main() {
    const vector<string> strings = {"cat", "dog", "dog", "horse"};

    const vector<string> requests = {"bear", "cat", "deer", "dog", "dogs", "horses"};

    // последовательные версии
    {
        LOG_DURATION("1");
    cout << "Request [" << requests[0] << "] → position "
         << LowerBound(strings.begin(), strings.end(), requests[0]) - strings.begin() << endl;
    cout << "Request [" << requests[1] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[1])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[2] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[2])
            - strings.begin()
         << endl;
    }
    // параллельные
    {
        LOG_DURATION("2");
        cout << "Request [" << requests[3] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[3])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[4] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[4])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[5] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[5])
            - strings.begin()
         << endl;
    }
}
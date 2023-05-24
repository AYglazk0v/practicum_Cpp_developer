#pragma once

#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

std::vector<std::string> SplitIntoWords(const std::string& text);
std::vector<std::string_view> SplitIntoWordsView(std::string_view text);

template <typename StringContainer>
std::set<std::string, std::less<>> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    std::set<std::string, std::less<>> non_empty_strings;
    for (auto str : strings) {
        if (!str.empty()) {
            non_empty_strings.emplace(str);
        }
    }
    return non_empty_strings;
}

inline bool IsValidWord(std::string_view word) {
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
              return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
          });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> ret;
    for (auto it = tokens.begin(), ite = tokens.end(); it != ite;) {
        auto stop = FindSentenceEnd(it, ite);
        Sentence<Token> senence{std::make_move_iterator(it), std::make_move_iterator(stop)};
        ret.push_back(std::move(senence));
        it = stop;
    }
    return ret;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}}));

    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}}));

    assert(SplitIntoSentences(vector<TestToken>(
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true}}))
           == vector<Sentence<TestToken>>({
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
               {{"Without"s}, {"copies"s}, {"."s, true}},
           }));
}

struct TestTokenNonCopy {
    string data;
    bool is_end_sentence_punctuation = false;

    TestTokenNonCopy(std::string data_, bool is_end = false) : data(data_), is_end_sentence_punctuation(is_end){}

    TestTokenNonCopy(const TestTokenNonCopy&) = delete;
    TestTokenNonCopy& operator=(const TestTokenNonCopy&) = delete;

    TestTokenNonCopy(TestTokenNonCopy&&) = default;
    TestTokenNonCopy& operator=(TestTokenNonCopy&&) = default;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }

    bool operator==(const TestTokenNonCopy& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestTokenNonCopy& token) {
    return stream << token.data;
}

void TestSplittingNonCopy() {
    vector<TestTokenNonCopy> test;
    test.push_back("Split"s);
    test.push_back("into"s);
    test.push_back("sentences"s);
    test.push_back("!"s);

    auto ret_val = SplitIntoSentences(std::move(test));

    vector<Sentence<TestTokenNonCopy>> assert_val;
    
    vector<TestTokenNonCopy> test_;
    test_.push_back("Split"s);
    test_.push_back("into"s);
    test_.push_back("sentences"s);
    test_.push_back("!"s);
    
    assert_val.push_back(std::move(test_));

    assert(ret_val == assert_val);
}

int main() {
    TestSplitting();
    TestSplittingNonCopy();
}
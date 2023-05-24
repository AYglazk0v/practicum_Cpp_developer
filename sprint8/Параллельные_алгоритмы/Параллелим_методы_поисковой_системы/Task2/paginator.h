#pragma once
#include <iostream>
#include <vector>

template <typename Iterator>
class Paginator {
    public:
        Paginator(Iterator it, Iterator ite, size_t page_size) {
            std::vector<std::vector<typename Iterator::value_type>> tmp_pages(
                std::distance(it, ite) / page_size + 1,
                std::vector<typename Iterator::value_type>(page_size)
            );
            for (size_t i = 0; it != ite; ++i) {
                for (size_t j = 0; it != ite && j != page_size; ++j, ++it) {
                    tmp_pages[i][j] = (*it);
                }
            }
            pages_ = std::move(tmp_pages);
        }

        std::vector<std::vector<typename Iterator::value_type>>::const_iterator begin() const {
            return pages_.begin();
        }
    
        std::vector<std::vector<typename Iterator::value_type>>::const_iterator end() const {
            return pages_.end();
        }

    private:
        std::vector<std::vector<typename Iterator::value_type>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const std::vector<typename Iterator::value_type>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}

#pragma once

#include <vector>

using namespace std;

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    PtrVector(const PtrVector& other) {
        items_.reserve(other.items_.size());

        try {
            for (auto p : other.items_) {
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение
                items_.push_back(p_copy);
            }
        } catch (...) {
            DeleteItems();
            throw;
        }
    }

    void swap(PtrVector& other) noexcept {
        items_.swap(other.items_);
    }

    ~PtrVector() {
        DeleteItems();
    }

    PtrVector& operator=(const PtrVector& rhs) {
        if (this != &rhs) {
            PtrVector rhs_copy{rhs};
            swap(rhs_copy);
        }
        return *this;
    }

    vector<T*>& GetItems() noexcept {
        return items_;
    }

    vector<T*> const& GetItems() const noexcept {
        return items_;
    }

private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

    vector<T*> items_;
};
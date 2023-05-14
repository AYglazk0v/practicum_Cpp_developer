#pragma once

#include <vector>

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    PtrVector(const PtrVector& other) {
        items_.reserve(other.items_.size());
        try {
            for (auto p : other.items_) {
                auto p_copy = p ? new T(*p) : nullptr;
                items_.push_back(p_copy);
            }
        } catch (...) {
            DeleteItems();
            throw;
        }
    }

    ~PtrVector() {
        DeleteItems();
    
    }
    std::vector<T*>& GetItems() noexcept {
        return items_;
    }

    std::vector<T*> const& GetItems() const noexcept {
        return items_;
    }

private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

private:
    std::vector<T*> items_;
};
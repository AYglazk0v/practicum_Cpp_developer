#pragma once

#include <array>
#include <stdexcept>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    using Iterator = std::array<T, N>::iterator;
    using ConstIterator = std::array<T, N>::const_iterator;

    explicit StackVector(size_t a_size = 0) {
        if (a_size > N) {
            throw std::invalid_argument("");
        }
        size_ = a_size;
    }

    T& operator[](size_t index) {
        return vector_[index];
    }
    const T& operator[](size_t index) const {
        return vector_[index];
    }

    Iterator begin()                      { return vector_.begin(); }
    Iterator end()                        { return vector_.end(); }
    Iterator begin() const                { return vector_.cbegin(); }
    Iterator end() const                  { return vector_.cend(); }

    size_t Size() const                   { return size_; }
    size_t Capacity() const               { return capacity_; }

    void PushBack(const T& value) {
        if (size_ >= capacity_) {
            throw std::overflow_error("");
        }
        vector_[++size_] = value;
    }
    
    T PopBack() {
        if (size_ == 0 ) {
            throw std::underflow_error("");
        }
        return vector_[size_--];
    }

private:
    size_t              size_;
    size_t              capacity_ = N;
    std::array<T, N>    vector_;
};

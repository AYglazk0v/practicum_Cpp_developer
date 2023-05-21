#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

#include <iostream>

#include "array_ptr.h"

class ReserveProxyObj {
public:
	ReserveProxyObj(size_t capacity) : capacity_(capacity) {   
    }

    size_t GetCapacity() const { return capacity_; }

private:
	size_t capacity_;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

private:
    size_t              size_ = 0;
    size_t              capacity_ = 0;
    ArrayPtr<Type>      array_;

public:
    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : size_(size), capacity_(size_), array_(size_) {
        std::generate(array_.Get(), array_.Get() + size_, []{return Type();});
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size_), array_(size_) {
        std::fill(begin(), end(), value);
    }
    
    SimpleVector(const SimpleVector& other) : size_(other.GetSize()), capacity_(other.GetCapacity()), array_(other.GetSize()) {
		std::copy(other.begin(), other.end(), begin());
	}

    SimpleVector(std::initializer_list<Type> init) : size_(init.size()), capacity_(size_), array_(size_) {
        std::copy(init.begin(), init.end(), begin());
    }

    SimpleVector(SimpleVector&& oth) {
        size_ = oth.size_;
        capacity_ = oth.capacity_;
        oth.size_ = 0;
        oth.capacity_ = 0;
        array_ = std::move(oth.array_);
    }
    
    SimpleVector(const ReserveProxyObj& obj) {
		Reserve(obj.GetCapacity());
	}

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (&rhs != this) {
            auto tmp = SimpleVector(rhs);
            swap(tmp);
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) {
        if (&rhs != this) {
            auto tmp = SimpleVector(rhs);
            swap(tmp);
        }
        return *this;
    }

    void PushBack(const Type& item) {
        if (size_ == capacity_) {
            if (!capacity_) {
                ++capacity_;
            }
            ArrayPtr<Type> tmp{capacity_ * 2};
            std::copy(begin(),end(),tmp.Get());
            array_.swap(tmp);
            capacity_ *= 2;
        }
        array_[size_++] = item;
    }
    
    void PushBack(Type&& item) {
        if (size_ == capacity_) {
            if (!capacity_) {
                ++capacity_;
            }
            ArrayPtr<Type> tmp{capacity_ * 2};
            std::move(begin(),end(),tmp.Get());
            array_.swap(tmp);
            capacity_ *= 2;
        }
        array_[size_++] = std::move(item);
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        Iterator it = const_cast<Iterator>(pos);
        if (capacity_ > size_) {
            std::copy_backward(it, end(), it + 1);
            *it = value;
            ++size_;
            return it;
        } else {
            SimpleVector<Type> tmp(capacity_ * 2);
            tmp.Resize(size_ + 1);
            std::copy(begin(), end(), tmp.begin());
            auto tmp_pos = std::distance(begin(), it);
            tmp[tmp_pos] = value;
            std::copy(it, end(), &tmp[tmp_pos + 1]);
            swap(tmp);
            return begin() + tmp_pos;
        }
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
        Iterator it = const_cast<Iterator>(pos);
        if (capacity_ > size_) {
            for (auto i = end(); i != it; i = std::prev(i)) {
                *i = std::move(*std::prev(i));
            }
            *it = std::move(value);
            ++size_;
            return it;
        } else {
            SimpleVector<Type> tmp(capacity_ * 2);
            tmp.Resize(size_ + 1);
            std::move(begin(), end(), tmp.begin());
            auto tmp_pos = std::distance(begin(), it);
            tmp[tmp_pos] = std::move(value);
            std::move(it, end(), &tmp[tmp_pos + 1]);
            swap(tmp);
            return begin() + tmp_pos;
            return it;
        }
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!IsEmpty()) {
            --size_;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        std::move(std::next(Iterator(pos)), end(), Iterator(pos));
        --size_;
        return Iterator(pos);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        array_.swap(other.array_);
        std::swap(size_, other.size_);
	    std::swap(capacity_, other.capacity_);
    }

    void swap(SimpleVector&& other) noexcept {
        SimpleVector tmp = std::move(other);
        other = std::move(this);
        this = std::move(tmp);
    }

    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        return *(array_.Get() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(array_.Get() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(array_.Get() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(array_.Get() + index);
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size < size_) {
            size_ = new_size;
        } else if (new_size <= capacity_) {
            std::generate(begin() + size_, begin() + new_size, [](){return Type();});
            size_ = new_size;
        } else {
            ArrayPtr<Type> tmp{new_size};
            std::move(begin(), end(), tmp.Get());
            std::generate(tmp.Get() + size_, tmp.Get() + new_size, [](){return Type();});
            array_.swap(tmp);
            size_ = new_size;
            capacity_ = new_size;
        }
    }
    
    void Reserve(size_t new_capacity){
        if (new_capacity > capacity_) {
            ArrayPtr<Type> tmp{new_capacity};
            std::copy(begin(), end(), tmp.Get());
            array_.swap(tmp);
            capacity_ = new_capacity;
        }
    }

    Iterator begin() noexcept               { return array_.Get(); }
    Iterator end() noexcept                 { return IsEmpty() ? begin() : array_.Get() + size_; }
    ConstIterator begin() const noexcept    { return array_.Get(); }
    ConstIterator end() const noexcept      { return IsEmpty() ? begin() : array_.Get() + size_; }
    ConstIterator cbegin() const noexcept   { return array_.Get(); }
    ConstIterator cend() const noexcept     { return IsEmpty() ? begin() : array_.Get() + size_; }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}
#pragma once

#include <cstddef>
#include <new>
#include <utility>

#include <iostream>

template<typename T>
class Vec {
    size_t size_;
    size_t cap_;
    T* data_;
public:
    class Iter {
        T* ptr_;
    public:
        Iter(T* p = nullptr) : ptr_{p} {}
        T* operator->() const {return ptr_;}
        T& operator*() const {return *ptr_;}
        Iter& operator+(size_t i) {ptr_ += i; return *this;}
        Iter& operator-(size_t i) {ptr_ -= i; return *this;}
    };

    Vec() : size_{0}, cap_{0}, data_{nullptr} {}
    ~Vec() noexcept {if (data_) operator delete(data_);}
    void reserve(size_t cap, size_t offset = 0) {
        if (cap <= cap_) return;
        T* ndata = (T*) operator new(cap * sizeof(T), std::align_val_t(alignof(T)));
        for (size_t i {0}; i < size_; ++i) {ndata[i + offset] = data_[i];}
        operator delete(data_);
        data_ = ndata;
        cap_ = cap;
    }
    void pushBack(const T& x) {
        if (size_ == cap_) {reserve(cap_ ? 2 * cap_ : 1);}
        data_[size_++] = x;
    }
    void popBack() {data_[--size_].~T();}
    void pushFront(const T& x) {
        if (size_ == cap_) {reserve(cap_ ? 2 * cap_ : 1, 1);}
        data_[0] = x;
        ++size_;
    }
    void popFront() {
        data_[0].~T();
        for (size_t i {1}; i < size_; ++i) data_[i-1] = data_[i];
        --size_;
    }
    T& front() const {return data_[0];}
    T& back() const {return data_[size_-1];}
    Iter begin() const {return Iter{data_};}
    Iter end() const {return Iter{data_ + size_};}
};
#ifndef INFRA_RB_H
#define INFRA_RB_H

#include <cstddef>
#include <atomic>
#include <new>
#include <iostream>
#include <sstream>

template<typename T>
class RingBuffer {
    const static size_t defCap = 16;
    size_t cap_;
    std::atomic_size_t beg_;
    std::atomic_size_t end_;
    T* data_;
public:
    explicit RingBuffer(size_t capacity) : cap_{capacity <= 0 ? defCap : capacity}, beg_{0}, end_{0} {
        data_ = static_cast<T*>(operator new(sizeof(T) * cap_, std::align_val_t(alignof(T))));
    }
    ~RingBuffer() noexcept {
        for (size_t i {0}; i < cap_; ++i) {
            data_[i].~T();
        }
        operator delete(data_);
    }
    void enqueue(T x) {
        size_t expEnd {end_.load()};
        while (!end_.compare_exchange_strong(expEnd, expEnd + 1, std::memory_order_release));
        new (&data_[wrap(expEnd)]) T{x};
        size_t expBeg {beg_.load()};
        // when full
        while (end_ > cap_ 
            && expBeg < end_ - cap_ 
            && !beg_.compare_exchange_strong(expBeg, end_ - cap_ + 1, std::memory_order_release));
    }
    bool dequeue(T& x) {
        if (empty()) return false;
        size_t expBeg {beg_.load()};
        while (!beg_.compare_exchange_strong(expBeg, expBeg + 1, std::memory_order_acquire));
        x = data_[wrap(expBeg)];
        return true;
    }
    std::stringstream ss() const {
        std::stringstream s;
        s << "size:" << size() << ",beg:" << beg_ << ",end:" << end_ << ' ';
        for (size_t i {0}; i < cap_; ++i) {
            s << '[' << data_[i] << ']';
        }
        return s;
    }

    size_t size() const {return end_ - beg_;}
    size_t cap() const {return cap_;}
    bool empty() const {return size() == 0;}
private:
    size_t wrap(size_t i) const {
        return i % cap_;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const RingBuffer<T>& rb) {
    return os << rb.ss().str();
}

#endif
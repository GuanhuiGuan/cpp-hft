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
    std::atomic_size_t size_;
    std::atomic_size_t start_;
    std::atomic_size_t end_;
    T* data_;
public:
    explicit RingBuffer(size_t capacity) : cap_{capacity <= 0 ? defCap : capacity} {
        data_ = static_cast<T*>(operator new(sizeof(T) * cap_, std::align_val_t(alignof(T))));
        size_.store(0);
        start_.store(0);
        end_.store(0);
    }
    ~RingBuffer() noexcept {
        for (size_t i {0}; i < size_; ++i) {
            data_[wrap(start_ + i)].~T();
            ++start_;
        }
        operator delete(data_);
    }
    void enqueue(T x) {
        size_t exEnd {end_.load()};
        while (!end_.compare_exchange_strong(exEnd, wrap(exEnd + 1), std::memory_order_acq_rel));
        if (size_ < cap_) ++size_;
        new (&data_[exEnd]) T{x};
    }
    T dequeue() {
        size_t exStart {start_.load()};
        while (!start_.compare_exchange_strong(exStart, wrap(exStart + 1), std::memory_order_acq_rel));
        T ret = data_[exStart];
        data_[exStart].~T();
        --size_;
        return ret;
    }
    std::stringstream ss() const {
        std::stringstream s;
        s << '[';
        for (size_t i {0}; i < size_; ++i) {
            s << data_[wrap(start_ + i)] << ((i == size_-1) ? "" : ",");
        }
        s << ']';
        return s;
    }

    size_t size() const {return size_;}
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
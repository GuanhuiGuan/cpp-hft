#ifndef INFRA_RB_H
#define INFRA_RB_H

#include <cstddef>
#include <atomic>
#include <new>
#include <iostream>
#include <sstream>
#include <cassert>

template<typename T>
struct Cell {
    std::atomic_size_t ver_;
    T data_;
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const Cell<T>& cell) {
    return os << '[' << cell.data_ << ':' << cell.ver_ << ']';
}

template<typename T>
class RingBuffer {
    const static size_t defCap = 16;
    size_t cap_;
    size_t mask_;
    std::atomic_size_t read_;
    std::atomic_size_t write_;
    Cell<T>* data_;
public:
    explicit RingBuffer(size_t capacity) : cap_{capacity <= 0 ? defCap : capacity}, mask_{cap_-1} {
        assert(cap_ >= 2 && (cap_ & (cap_-1)) == 0); // cap must be power of 2
        data_ = new Cell<T>[cap_];
        for (size_t i {0}; i < cap_; ++i) data_[i].ver_ = i;
        read_.store(0, std::memory_order_relaxed);
        write_.store(0, std::memory_order_relaxed);
    }
    ~RingBuffer() noexcept {
        delete[] data_;
    }
    bool enqueue(const T& x) {
        size_t write = write_.load(std::memory_order_relaxed);
        Cell<T>* cell;
        while (true) {
            cell = &data_[wrap(write)];
            size_t cellVer = cell->ver_.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)cellVer - (intptr_t)write;
            if (diff == 0) {
                if (write_.compare_exchange_weak(write, write+1, std::memory_order_relaxed)) {
                    break;
                }
            } else if (diff < 0) { // full
                return false;
            } else {
                write = write_.load(std::memory_order_relaxed);
            }
        }
        cell->data_ = x;
        cell->ver_.store(write+1, std::memory_order_release);
        return true;
    }
    bool dequeue(T& x) {
        size_t read = read_.load(std::memory_order_relaxed);
        Cell<T>* cell;
        while (true) {
            cell = &data_[wrap(read)];
            size_t cellVer = cell->ver_.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)cellVer - (intptr_t)(read+1); // b/c after write, version increments
            if (diff == 0) {
                if (read_.compare_exchange_weak(read, read+1, std::memory_order_relaxed)) break;
            } else if (diff < 0) {
                return false;
            } else {
                read = read_.load(std::memory_order_relaxed);
            }
        }
        x = cell->data_;
        cell->ver_.store(read + cap_, std::memory_order_release);
        return true;
    }
    std::stringstream ss() const {
        std::stringstream s;
        s << "size:" << size() << ",read:" << read_ << ",write:" << write_ << ' ';
        for (size_t i {0}; i < cap_; ++i) {
            s << '[' << data_[i] << ']';
        }
        return s;
    }
    size_t size() const {return write_.load(std::memory_order_relaxed) - read_.load(std::memory_order_relaxed);}
    size_t cap() const {return cap_;}
private:
    size_t wrap(size_t i) const {
        return (i & mask_) ;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const RingBuffer<T>& rb) {
    return os << rb.ss().str();
}

#endif
#pragma once

#include <utility>

struct Cnb {
    size_t rc_;
    Cnb(size_t rc = 1) : rc_{rc} {}
};

template<typename T>
class SP {
    T* p_;
    Cnb* cnb_;
public:
    explicit SP(T* p = nullptr) : p_{p}, cnb_{new Cnb()} {}
    ~SP() noexcept {
        if (cnb_) {
            if (--(cnb_->rc_) == 0) {
                delete p_;
                delete cnb_;
            }
        }
    }
};
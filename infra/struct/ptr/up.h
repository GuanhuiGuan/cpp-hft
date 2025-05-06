#pragma once

#include <utility>

template<typename T>
class UP {
    T* p_;
public:
    explicit UP(T* p = nullptr) : p_{p} {}
    ~UP() noexcept {if (p_) delete p_;}
    UP(const UP& up) = delete;
    UP& operator=(const UP& up) = delete;
    UP(UP&& up) noexcept {
        p_ = up.p_;
        up.p_ = nullptr;
    }
    UP& operator=(UP&& up) noexcept {
        if (*this == up) return *this;
        p_ = up.p_;
        up.p_ = nullptr;
        return *this;
    }
    operator bool() const {return p_;}
    T* operator->() const {return p_;}
    T& operator*() const {return *p_;}
};

template<typename T, typename... Args>
UP<T> makeUP(Args&&... args) {
    return UP<T>(new T(std::forward<Args>(args)...));
}
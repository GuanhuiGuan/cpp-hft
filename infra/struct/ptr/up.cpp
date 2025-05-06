#include "up.h"
#include <iostream>

struct A {
    bool b_;
    int i_;
    A(bool b, int i): b_{b}, i_{i} {}
};

int main() {
    int* x = new int(10);
    UP<int> u{x};
    UP<int> v = std::move(u);
    std::cout << *v << '\n';
    u = std::move(v);
    std::cout << v << '\n';
    std::cout << *u << '\n';

    UP<A> w = makeUP<A>(true, 42);
    std::cout << w->i_ << '\n';

    return 0;
}
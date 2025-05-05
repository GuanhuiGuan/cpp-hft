#include "allocator.h"

Allocator alloc(0, 8);

struct Base {
    void* operator new(size_t size) {
        return alloc.allocate(size);
    }

    void operator delete(void* ptr) {
        return alloc.deallocate(ptr);
    }
};

struct Small : public Base {
    bool b;
    Small(bool bo = true) : b{bo} {}
};

struct Large : public Base {
    long long a;
    long long b;
    Large(long long x, long long y): a{x}, b{y} {}
};

int main() {
    Small* s0 = new Small;
    std::cout << s0->b << '\n';

    Large* l0 = new Large(5, 7);
    std::cout << l0->a << ',' << l0->b << '\n';
    delete l0;

    delete s0;
    s0 = new Small(false);
    std::cout << s0->b << '\n';

    l0 = new Large(10, 11);
    std::cout << l0->a << ',' << l0->b << '\n';
    delete l0;

    delete s0;

    return 0;
}
#include "vec.h"
#include <iostream>

#include <vector>

struct A {
    int x = 42;
    ~A() {
        std::cout << "destroying\n";
        x = 0;
    }
};

int main() {

    Vec<A> v;
    v.pushBack({});
    v.pushBack({});
    A& a = v.back();
    std::cout << a.x << '\n';
    std::cout << (v.end()-1)->x << '\n';
    v.popBack();
    std::cout << a.x << '\n';
    v.pushFront({});
    A& b = v.front();
    std::cout << b.x << '\n';
    std::cout << v.begin()->x << '\n';
    v.popFront();
    std::cout << b.x << '\n';

    return 0;
}
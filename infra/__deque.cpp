#include <iostream>

#include "deque.h"

// #include <deque>

int main() {
    Deq<int> deq(3, 2);
    int x;
    std::cout << std::boolalpha;
    for (int i {10}; i >= 5; --i) {
        std::cout << "pushf: " << deq.pushFront(i) << ":" << i << '\n';
        // deq.print();
    }
    auto it = deq.end();
    std::cout << *it << '\n';
    for (int i {100}; i < 105; ++i) {
        std::cout << "pushb: " << deq.pushBack(i) << ":" << i << '\n';
    }
    std::cout << *it << '\n';
    for (int i {0}; i < 3; ++i) {
        std::cout << "popf: " << deq.popFront(x) << ":" << x << '\n';
    }
    for (int i {0}; i < 5; ++i) {
        std::cout << "popb: " << deq.popBack(x) << ":" << x << '\n';
    }

    // std::deque<int> d {11, 12, 13, 14, 15};
    // auto it = d.begin();
    // d.push_front(10);
    // std::cout << *it << '\n';

    return 0;
}
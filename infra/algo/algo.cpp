#include <vector>
#include <iostream>

template<typename Iter, typename Pred>
Iter findIf(Iter beg, Iter end, Pred pred) {
    while (beg != end) {
        if (pred(*beg)) return beg;
        ++beg;
    }
    return end;
}

constexpr uint32_t findNextPower2(uint32_t n) {
    --n; // for those n already being power of 2
    // if n==1, all bits are always 0, then ++n==1 which is 2^0
    n |= n >> 1; 
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16; // make all bits == 1
    ++n;
    return n;
}

int main() {

    std::cout << findNextPower2(18) << '\n';
    std::cout << findNextPower2(1) << '\n';

    return 0;
}
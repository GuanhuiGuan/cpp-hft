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

int main() {

    return 0;
}
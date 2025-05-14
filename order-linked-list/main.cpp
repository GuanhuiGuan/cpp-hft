#include "book.h"

int main() {
    using namespace kwan;
    OrderBook book{};
    
    book.insert(10, 110, false);
    book.insert(20, 40, false);
    book.insert(10, 50, true);
    book.insert(10, 20, true);
    book.insert(5, 20, false);
    book.insert(10, 100, true);

    book.match();

    return 0;
}
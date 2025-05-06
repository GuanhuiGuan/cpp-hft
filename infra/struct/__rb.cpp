#include "ringbuffer.h"
#include <sstream>

int main() {
    RingBuffer<std::string> rb(8);
    std::cout << rb << '\n';
    for (int i {0}; i < 3; ++i) rb.enqueue((std::stringstream() << 's' << i).str());
    std::cout << rb << '\n';
    for (int i {20}; i < 45; ++i) rb.enqueue((std::stringstream() << 's' << i).str());
    std::cout << rb << '\n';
    std::string s;
    while (rb.dequeue(s)) std::cout << '[' << s << ']';
    std::cout << '\n';
    std::cout << rb << '\n';
    rb.enqueue("str");
    std::cout << rb << '\n';

    return 0;
}
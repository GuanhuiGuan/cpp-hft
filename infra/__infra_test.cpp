#include "ringbuffer.h"
#include <sstream>

int main() {
    RingBuffer<std::string> rb(5);
    std::cout << rb << '\n';
    for (int i {0}; i < 3; ++i) rb.enqueue((std::stringstream() << 's' << i).str());
    std::cout << rb << '\n';
    for (int i {40}; i < 45; ++i) rb.enqueue((std::stringstream() << 's' << i).str());
    std::cout << rb << '\n';
    std::string s;
    while (!rb.empty()) std::cout << rb.dequeue(s) << ':' << s << ' ';
    std::cout << '\n';
    std::cout << rb << '\n';
    rb.enqueue("str");
    std::cout << rb << '\n';

    return 0;
}
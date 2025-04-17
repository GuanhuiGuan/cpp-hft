#include "ringbuffer.h"

int main() {
    RingBuffer<int> rb(8);
    for (int i {0}; i < 10; ++i) rb.enqueue(i);
    std::cout << rb.size() << '\n';
    std::cout << rb << '\n';
    while (!rb.empty()) std::cout << rb.dequeue() << ' ';
    std::cout << rb << '\n';

    return 0;
}
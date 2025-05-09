#include <iostream>
#include <bitset>

struct A {
    alignas(64) long long x[10];
};

int main() {
    std::cout << sizeof(A);

    return 0;
}
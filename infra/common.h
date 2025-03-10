#ifndef INFRA_COMMON
#define INFRA_COMMON

#include <chrono>
#include <memory>
#include <iostream>

namespace kwan {

using steady_clock = std::chrono::steady_clock;
using time_point = steady_clock::time_point;

}

#endif
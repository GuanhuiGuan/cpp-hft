#include "struct.hpp"
#include <iostream>

int main() {
    TradingEngine engine(5);
    engine.onOrder(TradingEngine::Order{0, 10});
    engine.onOrder(TradingEngine::Order{0, 20});
    engine.onOrder(TradingEngine::Order{0, 15});
    engine.onOrder(TradingEngine::Order{0, 16});

    auto [has, price] = engine.onNotify(0, 3);
    std::cout << has << ',' << price << '\n';

    return 0;
}
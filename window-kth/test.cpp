#include "engine.hpp"
#include <iostream>

int main() {
    TradingEngine engine(5);
    engine.onOrder(TradingEngine::Order{0, 10});
    engine.onOrder(TradingEngine::Order{1, 20});
    engine.onOrder(TradingEngine::Order{3, 15});
    engine.onOrder(TradingEngine::Order{6, 16});

    std::cout << std::boolalpha;
    auto [has, price] = engine.onNotify(6, 3);
    std::cout << has << ',' << price << '\n';
    auto [has1, price1] = engine.onNotify(7, 3);
    std::cout << has1 << ',' << price1 << '\n';
    auto [has2, price2] = engine.onNotify(11, 1);
    std::cout << has2 << ',' << price2 << '\n';

    // std::vector<int> v {4, 6, 3, 5, 9, 10, 12};
    // auto x = getMedian(v, 0, v.size()-1, std::less<>());
    // std::cout << x << '\n';
    // for (int e : v) std::cout << e << ',';

    return 0;
}
#include "orderbook.h"

int main() {
    OrderBook book;
    book.addOrder(OrderType::BUY, 1000, 100);
    book.addOrder(OrderType::BUY, 1200, 100);
    book.addOrder(OrderType::BUY, 1000, 20);
    std::cout << book.getTop(OrderType::BUY) << '\n';

    book.addOrder(OrderType::SELL, 1000, 100);
    book.addOrder(OrderType::SELL, 1200, 100);
    book.addOrder(OrderType::SELL, 1000, 20);
    std::cout << book.getTop(OrderType::SELL) << '\n';

    return 0;
}
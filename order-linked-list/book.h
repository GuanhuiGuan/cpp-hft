#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "order.h"

namespace kwan {

class OrderBook {

    double totalRevenue {0};

public:

    // TODO handle concurrency
    // virtual heads
    Order* buyOrders = new Order{-1, -1, true}; // descending in price
    Order* sellOrders = new Order{-2, -2, false}; // ascending in price

    ~OrderBook() {
        while (buyOrders) buyOrders = popOrder(buyOrders);
        while (sellOrders) sellOrders = popOrder(sellOrders);
    }

    void insert(int qty, double price, bool isBuy) {
        Order* orders = isBuy ? buyOrders : sellOrders;
        while (orders->next && (isBuy ? orders->next->price > price : orders->next->price < price)) {
            orders = orders->next;
        }
        Order* newOrder = new Order{qty, price, isBuy};
        newOrder->prev = orders;
        newOrder->next = orders->next;
        if (orders->next) orders->next->prev = newOrder;
        orders->next = newOrder;
    }

    void match() {
        Order* toBuy = buyOrders->next;
        Order* toSell = sellOrders->next;
        while (toBuy && toSell) {
            if (toBuy->price >= toSell->price) {
                int qty = std::min(toBuy->qty, toSell->qty);
                toBuy->qty -= qty;
                toSell->qty -= qty;
                totalRevenue += qty * (toBuy->price - toSell->price);

                std::cout << "matched, qty=" << qty << ", buyPrice=" << toBuy->price << ", sellPrice=" << toSell->price << '\n';

                if (toBuy->qty == 0) {
                    toBuy = popOrder(toBuy);
                }
                if (toSell->qty == 0) {
                    toSell = popOrder(toSell);
                }
            } else {
                // no more valid matching
                break;
            }
        }

        std::cout << "total revenue: " << totalRevenue << '\n';
    }

private:
    Order* popOrder(Order* order) {
        // if (!order) return;
        if (order->prev) order->prev->next = order->next;
        if (order->next) order->next->prev = order->prev;
        Order* nextOrder = order->next;
        delete order;
        return nextOrder;
    }
};

}

#endif
#ifndef ORDER_ORDER_H
#define ORDER_ORDER_H

#include "../infra/struct/common.h"

namespace kwan {

class Order {
public:
    int qty {0};
    double price {0};

    bool isBuy;
    
    time_point timestamp;

    Order* prev {nullptr};
    Order* next {nullptr};

    Order(int qty, double price, bool isBuy)
    : qty{qty}, price{price}, isBuy{isBuy}, timestamp{steady_clock::now()} 
    {}
};

}

#endif
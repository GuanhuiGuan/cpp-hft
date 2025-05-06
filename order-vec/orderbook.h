#include <vector>
#include <unordered_map>

#include <iostream>

using UID = unsigned long long;
using PL = long long;
using QTY = unsigned long long;
using IDX = size_t;

enum class OrderType {
    BUY,
    SELL,
};
std::ostream& operator<<(std::ostream& os, const OrderType& type) {
    switch (type) {
        case OrderType::BUY: return os << "BUY";
        case OrderType::SELL: return os << "SELL";
        default: return os;
    }
}

struct Order {
    OrderType type_;
    UID id_;
    PL price_;
    QTY qty_;
    Order(OrderType t = OrderType::BUY, UID i = 0, PL p = 0, QTY q = 0) : type_{t}, id_{i}, price_{p}, qty_{q} {}
};
std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << '[' << order.type_ << ',' << order.id_ << ',' << order.price_ << ',' << order.qty_ << ']';
}

class OrderBook {
    std::vector<Order> bestBuys;
    std::vector<Order> bestSells;
    std::unordered_map<PL, IDX> buyPlMap;
    std::unordered_map<PL, IDX> sellPlMap;
public:
    void addOrder(OrderType type, PL price, QTY qty) {
        std::vector<Order>& orders = (OrderType::BUY == type) ? bestBuys : bestSells;
        std::unordered_map<PL, IDX> map = (OrderType::BUY == type) ? buyPlMap : sellPlMap;
        if (map.find(price) == map.end()) {
            insert(orders, map, type, price, qty);
        } else {
            orders[map[price]].qty_ += qty;
        }
    }
    Order getTop(OrderType type) {
        std::vector<Order>& orders = (OrderType::BUY == type) ? bestBuys : bestSells;
        return orders.back();
    }
private:
    void adjustPlMap(const std::vector<Order>& orders, std::unordered_map<PL, IDX>& map) {
        for (size_t i {0}; i < orders.size(); ++i) map[orders[i].price_] = i;
    }
    void insert(std::vector<Order>& orders, std::unordered_map<PL, IDX>& map, OrderType type, PL price, QTY qty) {
        // more efficient to push back
        size_t i {orders.size()};
        for (; i > 0; --i) {
            if (orders[i-1].price_ == price) {
                orders[i-1].qty_ += qty;
                return;
            }
            if (OrderType::BUY == type && orders[i-1].price_ < price) {
                break;
            } else if (OrderType::SELL == type && orders[i-1].price_ > price) {
                break;
            }
        }
        UID id = rand();
        if (orders.empty() || i == orders.size()) orders.emplace_back(type, id, price, qty);
        else {
            orders.insert(orders.begin() + i, {type, id, price, qty});
        }
        adjustPlMap(orders, map);
    }
};

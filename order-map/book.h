#include <map>
#include <unordered_map>
#include <vector>

#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    auto it = v.cbegin();
    while (it != v.cend()) {
        os << '[' << *it++ << ']';
    }
    return os;
}

using Pricelvl = long;
using Uid = unsigned long;

struct Order {
    Uid id;
    long qty;
    Pricelvl price;
    Order(Uid i, long q, Pricelvl p) : id{i}, qty{q}, price{p} {}
};
std::ostream& operator<<(std::ostream& os, const Order& o) {
    return os << '[' << o.id << ',' << o.qty << ',' << o.price << ']';
}
struct BuyOrder : public Order {
    BuyOrder(Uid i = 0, long q = 0, Pricelvl p = 0): Order(i, q, p) {}
};
struct SellOrder : public Order {
    SellOrder(Uid i = 0, long q = 0, Pricelvl p = 0): Order(i, q, p) {}
};

class OrderBook {
    // std::unordered_map<Uid, std::map<Pricelvl, BuyOrder>::iterator> buyIdMap;
    std::map<Pricelvl, BuyOrder, std::greater<Pricelvl>> buyMap; // highest buy
    // std::unordered_map<Uid, std::map<Pricelvl, SellOrder>::iterator> sellIdMap;
    std::map<Pricelvl, SellOrder, std::less<Pricelvl>> sellMap; // lowest sell
public:
    template<typename T>
    void addOrder(const T& order) {}
    template<>
    void addOrder(const BuyOrder& order) {
        auto [it, _] = buyMap.emplace(order.price, order);
        // buyIdMap.emplace(order.id, it);
    }
    template<>
    void addOrder(const SellOrder& order) {
        auto [it, _] = sellMap.emplace(order.price, order);
        // sellIdMap.emplace(order.id, it);
    }

    template<typename T>
    void delOrder(const T& order) {}
    template<>
    void delOrder(const BuyOrder& order) {
        buyMap.erase(order.price);
        // buyIdMap.erase(order.id);
    }
    template<>
    void delOrder(const SellOrder& order) {
        sellMap.erase(order.price);
        // sellIdMap.erase(order.id);
    }

    template<typename T>
    void modOrder(const T& order) {}
    template<>
    void modOrder(const BuyOrder& order) {
        buyMap[order.price].qty += order.qty;
    }
    template<>
    void modOrder(const SellOrder& order) {
        sellMap[order.price].qty += order.qty;
    }

    template<typename T>
    void getTopK(std::vector<T>& vec, size_t k) {}
    template<>
    void getTopK(std::vector<BuyOrder>& vec, size_t k) {
        vec.reserve(k);
        auto it = buyMap.begin();
        for (; k > 0 && it != buyMap.end(); --k, ++it) {
            vec.push_back(it->second);
        }
    }
    template<>
    void getTopK(std::vector<SellOrder>& vec, size_t k) {
        vec.reserve(k);
        auto it = sellMap.begin();
        for (; k > 0 && it != sellMap.end(); --k, ++it) {
            vec.push_back(it->second);
        }
    }
};
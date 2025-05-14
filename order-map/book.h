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
    std::map<Pricelvl, BuyOrder, std::greater<Pricelvl>> buyMap; // highest buy
    std::map<Pricelvl, SellOrder, std::less<Pricelvl>> sellMap; // lowest sell
    Pricelvl totalBought = 0;
    Pricelvl totalSold = 0;
public:
    template<typename T>
    void addOrder(T&& order) {}
    template<>
    void addOrder(BuyOrder&& order) {
        auto [it, _] = buyMap.emplace(order.price, order);
        while (!sellMap.empty()
            && it->second.price >= sellMap.begin()->first 
            && it->second.qty > 0) {
            
            if (it->second.qty >= sellMap.begin()->second.qty) {
                totalBought += sellMap.begin()->second.qty * it->first;
                totalSold += sellMap.begin()->second.qty * sellMap.begin()->first;

                it->second.qty -= sellMap.begin()->second.qty;
                if (it->second.qty == 0) buyMap.erase(it);
                sellMap.erase(sellMap.begin());
            } else {
                totalBought += it->second.qty * it->first;
                totalSold += it->second.qty * sellMap.begin()->first;

                sellMap.begin()->second.qty -= it->second.qty;
                buyMap.erase(it);
            }
        }
    }
    template<>
    void addOrder(SellOrder&& order) {
        auto [it, _] = sellMap.emplace(order.price, order);
        while (!buyMap.empty() 
            && it->second.price <= buyMap.begin()->second.price 
            && it->second.qty > 0) {
            
            if (it->second.qty >= buyMap.begin()->second.qty) {
                totalBought += buyMap.begin()->second.qty * buyMap.begin()->first;
                totalSold += buyMap.begin()->second.qty * it->first;

                it->second.qty -= buyMap.begin()->second.qty;
                if (it->second.qty == 0) sellMap.erase(it);
                buyMap.erase(buyMap.begin());
            } else {
                totalBought += it->second.qty * buyMap.begin()->first;
                totalSold += it->second.qty * it->first;

                buyMap.begin()->second.qty -= it->second.qty;
                sellMap.erase(it);
            }
        }
    }

    Pricelvl& getTotalBought() {return totalBought;}
    Pricelvl& getTotalSold() {return totalSold;}

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
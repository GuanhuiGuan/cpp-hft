#pragma once

#include <deque>

template<typename Iter>
Iter quickSelectPartition(Iter beg, Iter end) {
    auto pivotVal = *beg;
    Iter i = beg + 1;
    Iter j = beg - 1;
    for (; i != end; ++i) {
        if (*i < pivotVal) std::swap(*(++j), *i);
    }
    std::swap(*(++j), *beg);
    return j;
}

template<typename Iter>
Iter quickSelectK(Iter beg, Iter end, size_t k) {
    if (beg == end || k <= 0) return end;
    Iter part = quickSelectPartition(beg, end);
    if (part - beg == k - 1) return part;
    if (part - beg > k - 1) return quickSelectK(beg, part, k);
    return quickSelectK(part + 1, end, k - (part - beg) - 1);
}

class TradingEngine {
public:
    struct Order {
        long time;
        long price;
        Order(long t = 0, long p = 0): time{t}, price{p} {}
    };
    TradingEngine(long ws) : windowSize_{ws} {}
    void onOrder(Order&& order) {
        if (orders_.empty() || orders_.back().time < order.time) {
            orders_.push_back(order);
            return;
        }
        auto it = orders_.end() - 1;
        while (it != orders_.begin() - 1 && it->time > order.time) {
            --it;
        }
        orders_.insert(++it, order);
    }
    auto onNotify(long now, long k) {
        if (k <= 0) return std::make_pair(false, 0L);
        while (!orders_.empty() && orders_.front().time < now - windowSize_) orders_.pop_front();
        if (orders_.size() < k) return std::make_pair(false, 0L);
        return std::make_pair(true, quickSelect(k));
    }
private:
    const long windowSize_;
    std::deque<Order> orders_;
    std::vector<long> temp_;

    long quickSelect(long k) {
        temp_.clear();
        temp_.reserve(orders_.size());
        for (size_t i {0}; i < orders_.size(); ++i) temp_.push_back(orders_[i].price);

        return *quickSelectK(temp_.begin(), temp_.end(), k);
    }
};


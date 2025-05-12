#pragma once

#include <deque>

template<typename Container, typename Cmp>
void insertionSort(Container& vec, long beg, long end, const Cmp& cmp) {
    using T = Container::value_type;
    if (beg == end) return;
    for (long i = beg + 1; i <= end; ++i) {
        T vi = vec[i];
        long j = i;
        for (; j > 0; --j) {
            if (cmp(vi, vec[j-1])) {
                vec[j] = vec[j-1]; 
            } else {
                break;
            }
        }
        vec[j] = vi;
    }
}

template<typename Container, typename Cmp>
auto getMedian(Container& vec, long beg, long end, const Cmp& cmp) {
    using T = Container::value_type;
    if (beg == end) return vec[beg];
    if (end - beg <= 4) {
        insertionSort(vec, beg, end, cmp);
        return vec[beg + (end - beg)/2];
    }
    Container temp;
    for (long i = beg; i <= end; i += 5) {
        temp.push_back(getMedian(vec, i, std::fminl(i + 4, end), cmp));
    }
    return getMedian(temp, 0, temp.size()-1, cmp);
}

// end is a valid index
template<typename Container, typename Cmp>
auto quickSelectK(Container ct, long beg, long end, long idx, Cmp cmp) {
    if (beg == end) return ct[beg];
    // auto pv = ct[beg];
    auto pv = getMedian(ct, beg, end, cmp);
    auto low = beg, high = end;
    while (low <= high) {
        while (low <= high && cmp(ct[low], pv)) ++low;
        while (low <= high && cmp(pv, ct[high])) --high;
        if (low <= high) std::swap(ct[low++], ct[high--]);
    }
    // at this point, low = high+1 OR low = high+2
    if (idx <= high) return quickSelectK(ct, beg, high, idx, cmp);
    else if (idx >= low) return quickSelectK(ct, low, end, idx, cmp);
    return ct[idx];
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

        return quickSelectK(temp_, 0, temp_.size()-1, k-1, std::greater<>());
    }
};


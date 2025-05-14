#include "book.h"

int main() {
    OrderBook ob;
    ob.addOrder(BuyOrder(1UL, 10UL, 10L));
    ob.addOrder(BuyOrder(2UL, 40UL, 20L));
    ob.addOrder(SellOrder(3UL, 10UL, 30L));
    ob.addOrder(SellOrder(4UL, 50UL, 10L));
    ob.addOrder(BuyOrder(5UL, 70UL, 40L));

    std::cout << ob.getTotalBought() << '\n';
    std::cout << ob.getTotalSold() << '\n';

    std::vector<BuyOrder> bv;
    ob.getTopK(bv, 5);
    std::cout << bv << '\n';
    std::vector<SellOrder> sv;
    ob.getTopK(sv, 1);
    std::cout << sv << '\n';

    return 0;
}
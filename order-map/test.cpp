#include "book.h"

int main() {
    OrderBook ob;
    BuyOrder bo (1UL, 10UL, 100L);
    ob.addOrder(bo);
    bo = BuyOrder(2UL, 50UL, 99L);
    ob.addOrder(bo);
    SellOrder so (1UL, 10UL, 200L);
    ob.addOrder(so);
    so = SellOrder(2UL, 50UL, 199L);
    ob.addOrder(so);

    std::vector<BuyOrder> bv;
    ob.getTopK(bv, 5);
    std::cout << bv << '\n';
    std::vector<SellOrder> sv;
    ob.getTopK(sv, 1);
    std::cout << sv << '\n';

    return 0;
}
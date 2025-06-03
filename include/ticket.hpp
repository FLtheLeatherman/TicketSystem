#ifndef TICKET_HPP
#define TICKET_HPP

#include "utility.hpp"
#include "bpt.hpp"

class TicketManagement {
private:
    BPlusTree<Username, bool, 100>* bpt2;
    BPlusTree<TrainID, int, 100>* bpt5; // 所有 release 了的 train 的 id
    MemoryRiver<TicketInfo, 1>* mr1; // 所有 release 了的 train 的售票情况
    BPlusTree<Username, int, 100>* bpt8; // 每个用户的订单
    MemoryRiver<Order, 1>* mr2; // 每个用户的订单
    BPlusTree<Pair<TrainID, Date>, int, 50>* bpt9; // 只存处于 pending 状态的 Order_Queue
public:
    TicketManagement() = default;
    ~TicketManagement() = default;
    void initialize(BPlusTree<Username, bool, 100>*, BPlusTree<TrainID, int, 100>*, MemoryRiver<TicketInfo, 1>*, BPlusTree<Username, int, 100>*, MemoryRiver<Order, 1>*, BPlusTree<Pair<TrainID, Date>, int, 50>*);
    void clear();
    void buy_ticket(Username, TrainID, Date, int, Station, Station, bool);
    void query_order(Username);
    void refund_ticket(Username, int);
};

#endif
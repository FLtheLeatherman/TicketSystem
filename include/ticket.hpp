#ifndef TICKET_HPP
#define TICKET_HPP

#include "utility.hpp"
#include "bpt.hpp"

class TicketManagement {
private:
    BPlusTree<TrainID, TicketInfo, 50>* bpt5; // 所有 release 了的 train 的售票情况
    BPlusTree<Username, Order, 100>* bpt8; // 每个用户的订单
    BPlusTree<Pair<TrainID, Date>, Order, 50>* bpt9; // 只存处于 pending 状态的 Order_Queue
public:
    TicketManagement() = default;
    ~TicketManagement() = default;
    void initialize();
    void clear(Username, TrainID, Date, int, Station, Station, bool);
    void buy_ticket();
    void query_order(Username);
    void refund_ticket(Username, int);
};

#endif
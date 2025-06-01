#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "memory_river.hpp"
#include "bpt.hpp"
#include "utility.hpp"
#include "priority_queue.hpp"
#include <iostream>

class TrainManagement {
private:
    BPlusTree<TrainID, Train, 50>* bpt3; // 所有 train
    BPlusTree<TrainID, int, 100>* bpt5; // 所有 release 了的 train 的 id
    MemoryRiver<TicketInfo, 1>* mr1; // 所有 release 了的 train 的售票情况
    BPlusTree<Pair<Station, Station>, int, 100>* bpt6; // 包含站 (s, t) 的 Train
    BPlusTree<Station, int, 100>* bpt7; // 包含某个站的 Train
public:
    TrainManagement() = default;
    ~TrainManagement() = default;
    void initialize();
    void clear();
    void add_train(TrainID, int, int, std::string, std::string, Time, std::string, std::string, Date, Date, char);
    void delete_train(TrainID);
    void release_train(TrainID);
    void query_train(TrainID, Date);
    void query_ticket(Station, Station, Date, bool); // false for time, true for price
    void query_transfer(Station, Station, Date, bool);
};

#endif
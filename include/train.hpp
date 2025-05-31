#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "bpt.hpp"
#include "utility.hpp"

class TrainManagement {
private:
    BPlusTree<TrainID, Train, 50>* bpt3; // 所有 train
    BPlusTree<TrainID, TicketInfo, 50>* bpt5; // 所有 release 了的 train 的售票情况
    BPlusTree<Pair<Station, Station>, TrainID, 100>* bpt6; // 包含站 (s, t) 的 TrainID;
    BPlusTree<Station, TrainID, 100>* bpt7; // 包含某个站的 TrainID
public:
    TrainManagement() = default;
    ~TrainManagement() = default;
    void initialize();
    void clear();
    void add_train(TrainID, int, int, std::string, std::string, int, std::string, std::string, std::string, char);
    void delete_train(TrainID);
    void release_train(TrainID);
    void query_train(TrainID);
    void query_ticket(Station, Station, Date, bool); // false for time, true for price
    void query_transfer(Station, Station, Date, bool);
};

#endif
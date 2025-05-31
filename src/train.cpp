#include "train.hpp"

void TrainManagement::initialize() {
    bpt3->initialize("train");
    bpt5->initialize("ticket");
    bpt6->initialize("train_info1");
    bpt7->initialize("train_info2");
}
void TrainManagement::clear() {
    bpt3->clear();
    bpt5->clear();
    bpt6->clear();
    bpt7->clear();
}
void TrainManagement::add_train(TrainID trainID, int stationNum, int seatNum, std::string stations, std::string prices, Time startTime, std::string travelTimes, std::string stopoverTimes, Date saleStart, Date saleEnd, char type) {
    if (bpt3->contain(trainID)) {
        std::cout << -1 << '\n';
        return;
    }
    Train newTrain;
    newTrain.trainID = trainID;
    newTrain.stationNum = stationNum;
    newTrain.seatNum = seatNum;
    newTrain.startTime = startTime;
    newTrain.type = type;
    newTrain.saleStart = saleStart;
    newTrain.saleEnd = saleEnd;
    std::string tmp;
    int cnt = 0;
    for (int i = 0; i < stations.length(); ++i) {
        int j = i;
        while (j < stations.length() && stations[j] != '|') {
            tmp += stations[j];
            j++;
        }
        newTrain.stations[cnt++] = tmp;
        tmp = "";
    }
    cnt = 0;
    int prev = 0;
    newTrain.prices[cnt++] = 0;
    for (int i = 0; i < prices.length(); ++i) {
        int j = i;
        while (j < prices.length() && prices[j] != '|') {
            tmp += prices[j];
            j++;
        }
        prev += std::stoi(tmp);
        newTrain.prices[cnt++] = prev;
        tmp = "";
    }
    cnt = 0;
    prev = 0;
    newTrain.travelTimes[0] = 0;
    for (int i = 0; i < travelTimes.length(); ++i) {
        int j = i;
        while (j < travelTimes.length() && travelTimes[j] != '|') {
            tmp += travelTimes[j];
            j++;
        }
        prev += std::stoi(tmp);
        newTrain.prices[cnt++] = prev;
        tmp = "";
    }
    cnt = 0;
    prev = 0;
    newTrain.stopoverTimes[0] = 0;
    for (int i = 0; i < stopoverTimes.length(); ++i) {
        int j = i;
        while (j < stopoverTimes.length() && stopoverTimes[j] != '|') {
            tmp += stopoverTimes[j];
            j++;
        }
        prev += std::stoi(tmp);
        newTrain.prices[cnt++] = prev;
        tmp = "";
    }
    bpt3->insert(trainID, newTrain);
    std::cout << 0 << '\n';
}
void TrainManagement::delete_train(TrainID trainID) {
    if (!bpt3->contain(trainID) || bpt5->contain(trainID)) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<Train> res = bpt3->show(trainID);
    bpt3->erase(trainID, res[0]);
    std::cout << 0 << '\n';
}
void TrainManagement::release_train(TrainID trainID) {
    if (!bpt3->contain(trainID) || bpt5->contain(trainID)) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<Train> res = bpt3->show(trainID);
    TicketInfo newTicketInfo(res[0]);
    bpt5->insert(trainID, newTicketInfo);
    std::cout << 0 << '\n';
}
void TrainManagement::query_train(TrainID trainID, Date date) {
    if (!bpt3->contain(trainID)) {
        std::cout << -1 << '\n';
    }
    if (bpt5->contain(trainID)) {
        sjtu::vector<TicketInfo> res = bpt5->show(trainID);
        Date saleStart = res[0].train.saleStart, saleEnd = res[0].train.saleEnd;
        int days = getNumDay(saleStart, date);
        int days2 = getNumDay(date, saleEnd);
        if (days < 0 || days2 < 0) {
            std::cout << -1 << '\n';
        }
        std::cout << trainID << ' ' << res[0].train.type;
        int nowDate = getDateInt(date);
        int nowTime = getTimeInt(res[0].train.startTime);
        int num = res[0].train.stationNum;
        for (int i = 0; i < num; ++i) {
            std::cout << res[0].train.stations[i] << ' ';
            if (i == 0) {
                std::cout << "xx-xx xx:xx -> " << getDateString(nowDate) << ' ' << getTimeString(nowTime) << ' ' << res[0].train.prices[i] << ' ' << res[0].seat[days][i];
            } else if (i == num - 1) {

            } else {

            }
        }
    } else {

    }
}
void TrainManagement::query_ticket(Station, Station, Date, bool) {

}
void TrainManagement::query_transfer(Station, Station, Date, bool) {

}
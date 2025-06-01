#include "train.hpp"

void TrainManagement::initialize() {
    bpt3->initialize("train");
    bpt5->initialize("ticket");
    mr1->initialize("ticket_info");
    bpt6->initialize("train_info1");
    bpt7->initialize("train_info2");
}
void TrainManagement::clear() {
    bpt3->clear();
    bpt5->clear();
    mr1->clear();
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
        newTrain.travelTimes[cnt++] = prev;
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
        newTrain.stopoverTimes[cnt++] = prev;
        tmp = "";
    }
    newTrain.stopoverTimes[cnt++] = prev;
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
    int id;
    mr1->get_info(id, 1);
    id++;
    bpt5->insert(trainID, id);
    mr1->write(newTicketInfo, id);
    for (int i = 0; i < res[0].stationNum; ++i) {
        bpt7->insert(res[0].stations[i], id);
    }
    for (int i = 0; i < res[0].stationNum; ++i) {
        for (int j = i + 1; j < res[0].stationNum; ++j) {
            bpt6->insert(Pair<Station, Station>(res[0].stations[i], res[0].stations[j]), id);
        }
    }
    std::cout << 0 << '\n';
}
void TrainManagement::query_train(TrainID trainID, Date date) {
    if (!bpt3->contain(trainID)) {
        std::cout << -1 << '\n';
    }
    if (bpt5->contain(trainID)) {
        sjtu::vector<int> res = bpt5->show(trainID);
        TicketInfo now;
        mr1->read(now, res[0]);
        Date saleStart = now.train.saleStart, saleEnd = now.train.saleEnd;
        int days = getNumDay(saleStart, date);
        int days2 = getNumDay(date, saleEnd);
        if (days < 0 || days2 < 0) {
            std::cout << -1 << '\n';
        }
        std::cout << trainID << ' ' << now.train.type << '\n';
        int startDate = getDateInt(date);
        int startTime = getTimeInt(now.train.startTime);
        int num = now.train.stationNum;
        for (int i = 0; i < num; ++i) {
            int arrTime = startTime + now.train.travelTimes[i];
            int leaTime = arrTime + now.train.stopoverTimes[i];
            if (i > 0) arrTime += now.train.stopoverTimes[i - 1];
            int arrDate = startDate + arrTime / 1440;
            int leaDate = startDate + leaTime / 1440;
            std::cout << now.train.stations[i] << ' ';
            if (i == 0) {
                std::cout << "xx-xx xx:xx -> " << getDateString(leaDate) << ' ' << getTimeString(leaTime) << ' ' << now.train.prices[i] << ' ' << now.seat[days][i] << '\n';
            } else if (i == num - 1) {
                std::cout << getDateString(arrDate) << ' ' << getTimeString(arrTime) << " -> " << getDateString(leaDate) << ' ' << getTimeString(leaTime) << ' ' << now.train.prices[i] << ' ' << now.seat[days][i] << '\n';
            } else {
                std::cout << getDateString(arrDate) << ' ' << getTimeString(arrTime) << " -> xx-xx xx:xx " << now.train.prices[i] << ' ' << now.seat[days][i] << '\n';
            }
        }
    } else {
        sjtu::vector<Train> res = bpt3->show(trainID);
        Date saleStart = res[0].saleStart, saleEnd = res[0].saleEnd;
        int days = getNumDay(saleStart, date);
        int days2 = getNumDay(date, saleEnd);
        if (days < 0 || days2 < 0) {
            std::cout << -1 << '\n';
        }
        std::cout << trainID << ' ' << res[0].type << '\n';
        int startDate = getDateInt(date);
        int startTime = getTimeInt(res[0].startTime);
        int num = res[0].stationNum;
        for (int i = 0; i < num; ++i) {
            int arrTime = startTime + res[0].travelTimes[i];
            int leaTime = arrTime + res[0].stopoverTimes[i];
            if (i > 0) arrTime += res[0].stopoverTimes[i - 1];
            int arrDate = startDate + arrTime / 1440;
            int leaDate = startDate + leaTime / 1440;
            std::cout << res[0].stations[i] << ' ';
            if (i == 0) {
                std::cout << "xx-xx xx:xx -> " << getDateString(leaDate) << ' ' << getTimeString(leaTime) << ' ' << res[0].prices[i] << ' ' << res[0].seatNum << '\n';
            } else if (i == num - 1) {
                std::cout << getDateString(arrDate) << ' ' << getTimeString(arrTime) << " -> " << getDateString(leaDate) << ' ' << getTimeString(leaTime) << ' ' << res[0].prices[i] << ' ' << res[0].seatNum << '\n';
            } else {
                std::cout << getDateString(arrDate) << ' ' << getTimeString(arrTime) << " -> xx-xx xx:xx " << res[0].prices[i] << ' ' << res[0].seatNum << '\n';
            }
        }
    }
}
struct info1 {
    TrainID trainID;
    Date leaDate, arrDate;
    Time leaTime, arrTime;
    int price;
    int seat;
    int time;
    info1() = default;
    info1(TrainID trainID, Date leaDate, Date arrDate, Time leaTime, Time arrTime, int price, int seat, int time) {
        this->trainID = trainID;
        this->leaDate = leaDate;
        this->arrDate = arrDate;
        this->leaTime = leaTime;
        this->arrTime = arrTime;
        this->price = price;
        this->seat = seat;
        this->time = time;
    }
};
struct Comp1 {
    bool operator()(info1 a, info1 b) {
        if (a.time != b.time) {
            return a.time < b.time;
        } else {
            return a.trainID < b.trainID;
        }
    }
};
struct Comp2 {
    bool operator()(info1 a, info1 b) {
        if (a.price != b.price) {
            return a.price < b.price;
        } else {
            return a.trainID < b.trainID;
        }
    }
};
void TrainManagement::query_ticket(Station from, Station to, Date day, bool flag) {
    sjtu::vector<int> res = bpt6->show(Pair<Station, Station>(from, to));
    sjtu::vector<info1> ans;
    int dayNum = getDateInt(day);
    for (int i = 0; i < res.size(); ++i) {
        TicketInfo now;
        mr1->read(now, res[i]);
        int leaTime, arrTime;
        int pos1, pos2;
        for (int j = 0; j < now.train.stationNum; ++j) {
            if (now.train.stations[j] == from) {
                leaTime = now.train.travelTimes[i] + now.train.stopoverTimes[i];
                pos1 = j;
            } else if (now.train.stations[j] == to) {
                arrTime = now.train.travelTimes[i];
                if (i > 0) {
                    arrTime += now.train.stopoverTimes[i - 1];
                }
                pos2 = j;
            }
        }
        leaTime += getTimeInt(now.train.startTime), arrTime += getTimeInt(now.train.startTime);
        dayNum -= leaTime / 1440;
        Date leaDate = getDate(dayNum);
        if (getNumDay(now.train.saleStart, leaDate) < 0 || getNumDay(leaDate, now.train.saleEnd) < 0) continue;
        int price = now.train.prices[pos2] - now.train.prices[pos1];
        int dis = getNumDay(now.train.saleStart, leaDate);
        int seat = 100000;
        for (int j = pos1; j < pos2; ++j) {
            seat = std::min(seat, now.seat[dis][j]);
        }
        Date arrDate = getDate(dayNum + arrTime / 1440);
        ans.push_back(info1(now.train.trainID, leaDate, arrDate, getTime(leaTime), getTime(arrTime), price, seat, arrTime - leaTime));
    }
    std::cout << ans.size() << '\n';
    if (!flag) {
        sjtu::priority_queue<info1, Comp1> pq;
        for (info1 x: ans) pq.push(x);
        while (!pq.empty()) {
            info1 now = pq.top();
            pq.pop();
            std::cout << now.trainID << ' ' << from << ' ' << getDateString(now.leaDate) << ' ' << getTimeString(now.leaTime) << " -> " << to << ' ' << getDateString(now.arrDate) << ' ' << getTimeString(now.arrTime) << ' ' << now.price << ' ' << now.seat << '\n';
        }
    } else {
        sjtu::priority_queue<info1, Comp2> pq;
        for (info1 x: ans) pq.push(x);
        while (!pq.empty()) {
            info1 now = pq.top();
            pq.pop();
            std::cout << now.trainID << ' ' << from << ' ' << getDateString(now.leaDate) << ' ' << getTimeString(now.leaTime) << " -> " << to << ' ' << getDateString(now.arrDate) << ' ' << getTimeString(now.arrTime) << ' ' << now.price << ' ' << now.seat << '\n';
        }
    }
}
struct info2 {
    info1 train1, train2;
    info2() = default;
    info2(info1, info1);
    info2& operator =(const info2& other);
};
info2::info2(info1 train1, info1 train2) {
    this->train1 = train1;
    this->train2 = train2;
}
info2& info2::operator =(const info2& other) {
    this->train1 = other.train1;
    this->train2 = other.train2;
    return *this;
}
void update1(info2& x, info2 y, Station& mid, Station cur) {
    if (x.train1.time + x.train2.time < y.train1.time + y.train2.time) {
        x = y;
        mid = cur;
    }
    else if (x.train1.price + x.train2.price < y.train1.price + y.train2.price) {
        x = y;
        mid = cur;
    }
    else if (x.train1.trainID < y.train1.trainID) {
        x = y;
        mid = cur;
    }
    else if (x.train2.trainID < y.train2.trainID) {
        x = y;
        mid = cur;
    }
}
void update2(info2& x, info2 y, Station& mid, Station cur) {
    if (x.train1.price + x.train2.price < y.train1.price + y.train2.price) {
        x = y;
        mid = cur;
    }
    else if (x.train1.time + x.train2.time < y.train1.time + y.train2.time) {
        x = y;
        mid = cur;
    }
    else if (x.train1.trainID < y.train1.trainID) {
        x = y;
        mid = cur;
    }
    else if (x.train2.trainID < y.train2.trainID) {
        x = y;
        mid = cur;
    }
}
void TrainManagement::query_transfer(Station from, Station to, Date day, bool flag) {
    sjtu::vector<int> res1, res2;
    res1 = bpt7->show(from), res2 = bpt7->show(to);
    bool isEmpty = true;
    Station mid;
    info2 ans;
    for (int i = 0; i < res1.size(); ++i) {
        for (int j = 0; j < res2.size(); ++j) {
            TicketInfo train1, train2;
            mr1->read(train1, res1[i]), mr1->read(train2, res2[i]);
            int num1 = train1.train.stationNum, num2 = train2.train.stationNum;
            bool fl1 = false;
            int pos1, pos2;
            int startDate1 = getDateInt(train1.train.saleStart);
            int leaDate1;
            int leaTime1 = getTimeInt(train1.train.startTime);
            int seat1 = 100000;
            for (int k = 0; k < num1; ++k) {
                if (train1.train.stations[k] == from) {
                    fl1 = true;
                    leaTime1 += train1.train.travelTimes[i] + train1.train.stopoverTimes[i];
                    leaDate1 = getDateInt(day) - leaTime1 / 1440;
                    if (getNumDay(train1.train.saleStart, getDate(leaDate1)) < 0 || getNumDay(getDate(leaDate1) ,train1.train.saleEnd) < 0) break;
                    pos1 = k;
                    seat1 = std::min(seat1, train1.seat[leaDate1 - startDate1][k]);
                    continue;
                }
                if (fl1) {
                    Station cur = train1.train.stations[k];
                    int arrTime1 = train1.train.travelTimes[k];
                    int arrDate1 = leaDate1 + arrTime1 / 1440;
                    if (k > 0) arrTime1 += train1.train.stopoverTimes[k - 1];
                    int price1 = train1.train.prices[k] - train1.train.prices[pos1];
                    int time1 = arrTime1 - leaTime1;
                    info1 t1(train1.train.trainID, getDate(leaDate1), getDate(arrDate1), getTime(leaTime1), getTime(arrTime1), price1, seat1, time1);
                    bool fl2 = false;
                    int arrTime2;
                    int seat2 = 100000;
                    for (int l = num2 - 1; l >= 0; --l) {
                        if (train2.train.stations[l] == to) {
                            fl2 = true;
                            arrTime2 = train2.train.travelTimes[l];
                            if (i > 0) arrTime2 += train2.train.stopoverTimes[l - 1];
                            pos2 = l;
                            continue;
                        }
                        if (fl2 && train2.train.stations[l] == cur) {
                            int startDate2 = getTimeInt(train2.train.startTime);
                            int leaTime2 = startDate2 + train2.train.stopoverTimes[l] + train2.train.travelTimes[l];
                            int leaDate2 = startDate2 + leaTime2 / 1440;
                            if (leaTime2 % 1440 < arrTime1 % 1440) leaDate2 += 1;
                            if (getNumDay(train2.train.saleStart, getDate(leaDate2)) < 0 || getNumDay(getDate(leaDate2) ,train2.train.saleEnd) < 0) break;
                            int arrDate2 = startDate2 + arrTime2 / 1440;
                            int time = 1440 * (getNumDay(getDate(leaDate1), getDate(arrDate2)) - 1) + 1440 - leaTime1 % 1440 + arrTime2 % 1440;
                            int price2 = train2.train.prices[pos2] - train2.train.prices[l];
                            seat2 = std::min(seat2, train2.seat[leaDate2 - startDate2][l]);
                            info1 t2(train2.train.trainID, getDate(leaDate2), getDate(arrDate2), getTime(leaTime2), getTime(arrTime2), price2, seat2, arrTime2 - leaTime2);
                            info2 now(t1, t2);
                            if (isEmpty) ans = now, mid = cur;
                            else if (!flag) update1(ans, now, mid, cur);
                            else if (flag) update2(ans, now, mid, cur);
                        }
                    }
                    seat1 = std::min(seat1, train1.seat[leaDate1 - startDate1][k]);
                }
            }
        }
    }
    if (isEmpty) std::cout << 0 << '\n';
    else {
        std::cout << ans.train1.trainID << ' ' << from << ' ' << getDateString(ans.train1.leaDate) << ' ' << getTimeString(ans.train1.leaTime) << " -> " << mid << ' ' << getDateString(ans.train1.arrDate) << ' ' << getTimeString(ans.train1.arrTime) << ' ' << ans.train1.price << ' ' << ans.train1.seat << '\n';
        std::cout << ans.train2.trainID << ' ' << mid << ' ' << getDateString(ans.train2.leaDate) << ' ' << getTimeString(ans.train2.leaTime) << " -> " << to << ' ' << getDateString(ans.train2.arrDate) << ' ' << getTimeString(ans.train2.arrTime) << ' ' << ans.train2.price << ' ' << ans.train2.seat << '\n';
    }
}
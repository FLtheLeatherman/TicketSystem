#include "ticket.hpp"

void TicketManagement::initialize(BPlusTree<Username, bool, 100>* bpt2, BPlusTree<TrainID, int, 100>* bpt5, MemoryRiver<TicketInfo, 1>* mr1, BPlusTree<Username, int, 100>* bpt8, MemoryRiver<Order, 1>* mr2, BPlusTree<Pair<TrainID, Date>, int, 50>* bpt9) {
    this->bpt2 = bpt2;
    this->bpt5 = bpt5;
    this->mr1 = mr1;
    this->bpt8 = bpt8;
    this->mr2 = mr2;
    this->bpt9 = bpt9;
    bpt8->initialize("user_order");
    mr2->initialize("order_info_");
    bpt9->initialize("order_queue");
}
void TicketManagement::clear() {
    bpt8->clear();
    mr2->clear();
    bpt9->clear();
}
void TicketManagement::buy_ticket(Username username, TrainID trainID, Date date, int num, Station from, Station to, bool flag) {
    if (!bpt2->contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<int> id = bpt5->show(trainID);
    if (id.empty()) {
        std::cout << -1 << '\n';
        return;
    }
    TicketInfo cur;
    mr1->read(cur, id[0]);
    if (num > cur.train.seatNum) {
        std::cout << -1 << '\n';
        return;
    }
    int seat = 100000;
    bool fl1 = false;
    int startDate = getDateInt(cur.train.saleStart);
    int leaTime, arrTime;
    int pos1 = -1, pos2 = -1;
    for (int i = 0; i < cur.train.stationNum; ++i) {
        if (cur.train.stations[i] == from) {
            leaTime = cur.train.travelTimes[i] + cur.train.stopoverTimes[i];
            pos1 = i;
        }
        if (cur.train.stations[i] == to) {
            arrTime = cur.train.travelTimes[i];
            if (i > 0) arrTime += cur.train.stopoverTimes[i - 1];
            pos2 = i;
        }
    }
    if (pos1 == -1 || pos2 == -1 || pos1 > pos2) {
        std::cout << -1 << '\n';
        return;
    }
    // std::cout << pos1 << ' ' << pos2 << std::endl;
    leaTime += getTimeInt(cur.train.startTime), arrTime += getTimeInt(cur.train.startTime);
    int leaDate = getDateInt(date) - leaTime / 1440;
    if (getNumDay(cur.train.saleStart, getDate(leaDate)) < 0 || getNumDay(getDate(leaDate), cur.train.saleEnd) < 0) {
        std::cout << -1 << '\n';
        return;
    }
    int price = cur.train.prices[pos2] - cur.train.prices[pos1];
    // std::cout << trainID << ' ' << leaDate - startDate << std::endl;
    // std::string sb = "LeavesofGrass";
    // TrainID sbb; sbb = sb;
    // if (trainID == sbb && startDate + 31 == leaDate) {
    //     // std::cout << "???\n";
    //     std::cerr << "[1] buy_ticket " << "-u " << "I_am_the_admin" << " -i " << trainID << " -d " << getDateString(date) << " -n " << num << " -f " << from << " -t " << to << " -q " << (flag ? "true" : "false") << std::endl;
    //     // query_order(username);
    //     // std::cerr << cur.train.saleEnd << std::endl;
    //     // std::cerr << getNumDay(getDate(leaDate), cur.train.saleEnd) << std::endl;
    // }
    for (int i = pos1; i < pos2; ++i) {
        seat = std::min(seat, cur.seat[leaDate - startDate][i]);
    }
    Order newOrder(success, trainID, from, to, getDate(leaDate), date, getDate(leaDate + arrTime / 1440), getTime(leaTime), getTime(arrTime), price, num);
    if (seat >= num) {
        // std::cout << price << ' ' << num << std::endl;
        std::cout << price * num << '\n';
        for (int i = pos1; i < pos2; ++i) {
            cur.seat[leaDate - startDate][i] -= num;
            // std::cout << cur.seat[leaDate - startDate][i] << ' ';
        }
        // std::cout << std::endl;
        mr1->write(cur, id[0]);
        newOrder.status = success;
        int index;
        mr2->get_info(index, 1);
        index++;
        mr2->write(newOrder, index);
        bpt8->insert(username, index);
        mr2->write_info(index, 1);
    } else {
        if (flag) {
            std::cout << "queue\n";
            newOrder.status = pending;
            int index;
            mr2->get_info(index, 1);
            index++;
            mr2->write(newOrder, index);
            bpt8->insert(username, index);
            bpt9->insert(Pair<TrainID, Date>(trainID, getDate(leaDate)), index);
            mr2->write_info(index, 1);
        } else {
            std::cout << -1 << '\n';
        }
    }
    // for (int i = 0; i < cur.train.stationNum; ++i) {
    //     std::cout << cur.seat[leaDate - startDate][i] << ' ';
    // }
    // std::cout << std::endl;
}
void TicketManagement::query_order(Username username) {
    if (!bpt2->contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<int> res = bpt8->show(username);
    std::cout << res.size() << '\n';
    if (!res.size()) {
        // std::cout << "??" << std::endl;
        // int tmp = res.size() - 1;
        // std::cout << tmp << std::endl;
        return;
    }
    for (int i = res.size() - 1; i >= 0; --i) {
        Order cur;
        mr2->read(cur, res[i]);
        // std::cout << cur.trainID << ' ' << getDateString(cur.leavingDate) << std::endl; 
        std::cout << '[';
        if (cur.status == success) std::cout << "success";
        else if (cur.status == pending) std::cout << "pending";
        else if (cur.status == refunded) std::cout << "refunded";
        std::cout << ']' << ' ';
        int tmp = getDateInt(cur.leavingDate) + getTimeInt(cur.leavingTime) / 1440;
        std::cout << cur.trainID << ' ' << cur.from << ' ' << getDateString(getDate(tmp)) << ' ' << getTimeString(cur.leavingTime) << " -> " << cur.to << ' ' << getDateString(cur.arrivingDate) << ' ' << getTimeString(cur.arrivingTime) << ' ' << cur.price << ' ' << cur.num << '\n';
    }
}
void TicketManagement::refund_ticket(Username username, int index) {
    if (!bpt2->contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<int> res = bpt8->show(username);
    if (index > res.size()) {
        std::cout << -1 << '\n';
        return;
    }
    // std::cout << res.size() << std::endl;
    int id = res[res.size() - index];
    Order cur;
    mr2->read(cur, id);
    // std::cout << cur.status << std::endl;
    if (cur.status == refunded) {
        // std::cout << cur.trainID << ' ' << cur.startingDate << ' ' << cur.from << ' ' << cur.to << ' ' << cur.num << std::endl;
        // std::cout << cur.status << std::endl;
        // std::cout << -1 << 'c' << '\n';
        std::cout << -1 << '\n';
        return;
    }
    std::cout << 0 << '\n';
    TicketInfo curTicket;
    sjtu::vector<int> tmp = bpt5->show(cur.trainID);
    mr1->read(curTicket, tmp[0]);
    // std::cout << "refund!!" << index << std::endl;
    // query_order(username);
    // std::cout << cur.trainID << ' ' << cur.from << ' ' << cur.to << std::endl;
    TrainID trainID = cur.trainID;
    Date date = cur.startingDate;
    int startingDate = getDateInt(curTicket.train.saleStart), leaDate = getDateInt(date);
    int num = cur.num;
    bool fl = false;
    // std::cout << cur.trainID << ' ' << leaDate - startingDate << ' ' << num << std::endl;
    // std::string sb = "LeavesofGrass";
    // TrainID sbb; sbb = sb;
    // if (cur.trainID == sbb && startingDate + 31 == leaDate) {
    //     // std::cout << "???\n";
    //     std::cerr << "[2] refund_ticket -u" << ' ' << "I_am_the_admin" << " -n " << index << std::endl;
    //     query_order(username);
    // }
    if (cur.status == pending) {
        // std::cout << cur.trainID << ' ' << leaDate - startingDate << ' ' << num << std::endl;
        bpt9->erase(Pair<TrainID, Date>(trainID, date), id);
        cur.status = refunded;
        mr2->write(cur, id);
        return;
    }
    for (int i = 0; i < curTicket.train.stationNum; ++i) {
        if (curTicket.train.stations[i] == cur.from) fl = true;
        if (curTicket.train.stations[i] == cur.to) fl = false;
        if (fl) {
            // std::cout << "!" << i << std::endl;
            curTicket.seat[leaDate - startingDate][i] += num;
        }
        // std::cout << curTicket.train.stations[i] << ' ' << curTicket.seat[leaDate - startingDate][i] << '\n';
    }
    // std::cout << std::endl;
    bpt9->erase(Pair<TrainID, Date>(trainID, date), id);
    cur.status = refunded;
    mr2->write(cur, id);
    sjtu::vector<int> orders = bpt9->show(Pair<TrainID, Date>(trainID, date));
    for (int i = 0; i < orders.size(); ++i) {
        Order curOrder;
        mr2->read(curOrder, orders[i]);
        bool fl = false;
        int seat = 100000;
        for (int i = 0; i < curTicket.train.stationNum; ++i) {
            if (curTicket.train.stations[i] == curOrder.from) fl = true;
            if (curTicket.train.stations[i] == curOrder.to) fl = false;
            if (fl) seat = std::min(seat, curTicket.seat[leaDate - startingDate][i]);
        }
        if (seat >= curOrder.num) {
            // std::cout << "refund successfully, now buy" << curOrder.from << ' ' << curOrder.to << ' ' << curTicket.train.trainID << ' ' << getDateString(getDate(leaDate)) << ' ' << leaDate - startingDate << ' ' << seat << ' ' << curOrder.num << std::endl;
            for (int i = 0; i < curTicket.train.stationNum; ++i) {
                if (curTicket.train.stations[i] == curOrder.from) fl = true;
                if (curTicket.train.stations[i] == curOrder.to) fl = false;
                if (fl) curTicket.seat[leaDate - startingDate][i] -= curOrder.num;
            }
            curOrder.status = success;
            bpt9->erase(Pair<TrainID, Date>(trainID, date), orders[i]);
            mr2->write(curOrder, orders[i]);
        }
    }
    // for (int i = 0; i < curTicket.train.stationNum; ++i) {
    //     std::cout << curTicket.seat[leaDate - startingDate][i] << ' ';
    // }
    // std::cout << std::endl;
    mr1->write(curTicket, tmp[0]);
}
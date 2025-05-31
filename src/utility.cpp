#include "utility.hpp"

int getDateLen(Date start, Date end) {
    int res = 0;
    for (int i = start.first; i < end.first; ++i) {
        if (i == 6 || i == 9) res += 30;
        if (i == 7 || i == 8) res += 30;
    }
    res += end.second;
    res -= start.first - 1;
    return res;
}

User::User(Username username, Password password, Name name, MailAddress mailAddr, int privilege) {
    this->username = username;
    this->password = password;
    this->name = name;
    this->mailAddr = mailAddr;
    this->privilege = privilege;
}
bool User::operator <(const User& other) {
    return false;
}
bool User::operator ==(const User& other) {
    if (!(this->username == other.username)) return false;
    if (!(this->password == other.password)) return false;
    if (!(this->name == other.name)) return false;
    if (!(this->mailAddr == other.mailAddr)) return false;
    if (!(this->privilege == other.privilege)) return false;
    return true;
}

Train::Train(TrainID trainID, int stationNum, Station stations[maxStation], int seatNum, int prices[maxStation], Time startTime, int travelTimes[maxStation], int stopoverTimes[maxStation], Date saleStart, Date saleEnd, char type) {
    this->trainID = trainID;
    this->stationNum = stationNum;
    this->seatNum = seatNum;
    this->startTime = startTime;
    this->saleStart = saleStart;
    this->saleEnd = saleEnd;
    this->type = type;
    for (int i = 0; i < stationNum; ++i) {
        this->stations[i] = stations[i];
    }
    for (int i = 0; i < stationNum - 1; ++i) {
        this->prices[i] = prices[i];
        this->travelTimes[i] = travelTimes[i];
    }
    for (int i = 0; i < stationNum - 2; ++i) {
        this->stopoverTimes[i] = stopoverTimes[i];
    }
}
Train& Train::operator =(const Train& other) {
    this->trainID = trainID;
    this->stationNum = stationNum;
    this->seatNum = seatNum;
    this->startTime = startTime;
    this->saleStart = saleStart;
    this->saleEnd = saleEnd;
    this->type = type;
    for (int i = 0; i < this->stationNum; ++i) {
        this->stations[i] = other.stations[i];
    }
    for (int i = 0; i < this->stationNum - 1; ++i) {
        this->prices[i] = other.prices[i];
        this->travelTimes[i] = other.travelTimes[i];
    }
    for (int i = 0; i < this->stationNum - 2; ++i) {
        this->stopoverTimes[i] = other.stopoverTimes[i];
    }
    return *this;
}
bool Train::operator <(const Train& other) {
    return false;
}
bool Train::operator ==(const Train& other) {
    return this->trainID == other.trainID;
}

Order::Order(Status status, TrainID trainID, Station from, Station to, Date leavingDate, Date arrivingDate, Time leavingTime, Time arrivingTime, int price, int num) {
    this->status = status;
    this->trainID = trainID;
    this->from = from;
    this->to = to;
    this->leavingDate = leavingDate;
    this->arrivingDate = arrivingDate;
    this->leavingTime = leavingTime;
    this->arrivingTime = arrivingTime;
    this->price = price;
    this->num = num;
}
bool Order::operator <(const Order &other) {
    return false;
}
bool Order::operator ==(const Order &other) {
    if (!(this->status == other.status)) return false;
    if (!(this->trainID == other.trainID)) return false;
    if (!(this->from == other.from)) return false;
    if (!(this->to == other.to)) return false;
    if (!(this->leavingDate == other.leavingDate)) return false;
    if (!(this->arrivingDate == other.arrivingDate)) return false;
    if (!(this->leavingTime == other.leavingTime)) return false;
    if (!(this->arrivingTime == other.arrivingTime)) return false;
    if (!(this->price == other.price)) return false;
    if (!(this->num == other.num)) return false;
    return true;
}

TicketInfo::TicketInfo(Train train) {
    this->train = train;
    this->dateLen = getDateLen(train.saleStart, train.saleEnd);
    for (int i = 0; i < this->dateLen; ++i) {
        for (int j = 0; j < train.stationNum - 1; ++j) {
            this->seat[i][j] = train.seatNum;
        }
    }
}
TicketInfo::TicketInfo(const TicketInfo& other) {
    this->train = other.train;
    this->dateLen = other.dateLen;
    for (int i = 0; i < this->dateLen; ++i) {
        for (int j = 0; j < this->train.stationNum - 1; ++j) {
            this->seat[i][j] = other.seat[i][j];
        }
    }
}
bool TicketInfo::operator <(const TicketInfo& other) {
    return false;
}
bool TicketInfo::operator ==(const TicketInfo& other) {
    if (!(this->dateLen == other.dateLen)) return false;
    if (!(this->train == other.train)) return false;
    return true;
}
#include "utility.hpp"

int getNumDay(Date start, Date end) {
    // std::cout << start.first << ' ' << start.second << ' ';
    // std::cout << end.first << ' ' << end.second << ' ';
    if (start.first > end.first || (start.first == end.first && start.second > end.second)) {
        return -1;
    }
    int res = 0;
    for (int i = start.first; i < end.first; ++i) {
        if (i == 4 || i == 6 || i == 9 || i == 11) res += 30;
        if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12) res += 31;
        if (i == 2) res += 28;
    }
    if (start.first != end.first) {
        res += end.second;
        res -= start.second - 1;
    } else {
        res = end.second - start.second + 1;
    }
    // std::cout << res << std::endl;
    return res;
}
int getDateInt(Date date) {
    int res = 0;
    for (int i = 6; i < date.first; ++i) {
        if (i == 6 || i == 9 || i == 11) res += 30;
        if (i == 7 || i == 8 || i == 10 || i == 12) res += 31;
    }
    for (int i = 5; i >= date.first; --i) {
        if (i == 4) res -= 30;
        if (i == 1 || i == 3 || i == 5) res -= 31;
        if (i == 2) res -= 28;
    }
    res += date.second;
    return res;
}
Date getDate(int date) {
    Date res;
    if (date <= 30) {
        res.first = 6;
        res.second = date;
    } else if (date <= 61) {
        res.first = 7;
        res.second = date - 30;
    } else if (date <= 92) {
        res.first = 8;
        res.second = date - 61;
    } else if (date <= 122) {
        res.first = 9;
        res.second = date - 92;
    } else if (date <= 153) {
        res.first = 10;
        res.second = date - 122;
    } else if (date <= 183) {
        res.first = 11;
        res.second = date - 153;
    } else {
        res.first = 12;
        res.second = date - 183;
    }
    return res;
}
std::string getDateString(int date) {
    return getDateString(getDate(date));
}
std::string getDateString(Date date) {
    std::string res;
    if (date.first < 10) res += "0";
    res += std::to_string(date.first);
    res += "-";
    if (date.second < 10) res += "0";
    res += std::to_string(date.second);
    return res;
}
int getTimeInt(Time time) {
    return time.first * 60 + time.second;
}
Time getTime(int time) {
    Time res;
    time %= 1440;
    res.first = time / 60;
    res.second = time % 60;
    return res;
}
std::string getTimeString(int time) {
    return getTimeString(getTime(time));
}
std::string getTimeString(Time time) {
    std::string res;
    if (time.first < 10) res += "0";
    res += std::to_string(time.first);
    res += ":";
    if (time.second < 10) res += "0";
    res += std::to_string(time.second);
    return res;
}

User::User(Username username, Password password, Name name, MailAddress mailAddr, int privilege) {
    this->username = username;
    this->password = password;
    this->name = name;
    this->mailAddr = mailAddr;
    this->privilege = privilege;
}
User& User::operator =(const User& other) {
    this->username = other.username;
    this->password = other.password;
    this->name = other.name;
    this->mailAddr = other.mailAddr;
    this->privilege = other.privilege;
    return *this;
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
        this->prices[i] = prices[i];
        this->travelTimes[i] = travelTimes[i];
        this->stopoverTimes[i] = stopoverTimes[i];
    }
}
Train& Train::operator =(const Train& other) {
    // std::cout << "cnm" << std::endl;
    this->trainID = other.trainID;
    this->stationNum = other.stationNum;
    this->seatNum = other.seatNum;
    this->startTime = other.startTime;
    this->saleStart = other.saleStart;
    this->saleEnd = other.saleEnd;
    this->type = other.type;
    for (int i = 0; i < this->stationNum; ++i) {
        this->stations[i] = other.stations[i];
        this->prices[i] = other.prices[i];
        this->travelTimes[i] = other.travelTimes[i];
        this->stopoverTimes[i] = other.stopoverTimes[i];
    }
    // std::cout << "cnm" << std::endl;
    return *this;
}
bool Train::operator <(const Train& other) {
    return false;
}
bool Train::operator ==(const Train& other) {
    return this->trainID == other.trainID;
}

Order::Order(Status status, TrainID trainID, Station from, Station to, Date startingDate, Date leavingDate, Date arrivingDate, Time leavingTime, Time arrivingTime, int price, int num) {
    this->status = status;
    this->trainID = trainID;
    this->from = from;
    this->to = to;
    this->startingDate = startingDate;
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

TicketInfo::TicketInfo(const Train& train) {
    // std::cout << '?' << std::endl;
    this->train = train;
    this->dateLen = getNumDay(train.saleStart, train.saleEnd);
    // std::cout << dateLen << std::endl;
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
TicketInfo& TicketInfo::operator =(const TicketInfo& other) {
    this->train = other.train;
    this->dateLen = other.dateLen;
    for (int i = 0; i < this->dateLen; ++i) {
        for (int j = 0; j < this->train.stationNum - 1; ++j) {
            this->seat[i][j] = other.seat[i][j];
        }
    }
    return *this;
}
bool TicketInfo::operator <(const TicketInfo& other) {
    return false;
}
bool TicketInfo::operator ==(const TicketInfo& other) {
    if (!(this->dateLen == other.dateLen)) return false;
    if (!(this->train == other.train)) return false;
    return true;
}

std::string get_token(std::string command, int pos) {
    std::string res;
    while (pos < command.length() && command[pos] != ' ') {
        res += command[pos];
        pos++;
    }
    return res;
}
sjtu::vector<std::string> get_token_list(std::string command) {
    sjtu::vector<std::string> res;
    for (int i = 0; i < command.size(); ++i) {
        std::string tmp = "";
        int j = i;
        while (j < command.size() && command[j] != ' ') {
            tmp += command[j], j++;
        }
        res.push_back(tmp);
        i = j;
    }
    return res;
}
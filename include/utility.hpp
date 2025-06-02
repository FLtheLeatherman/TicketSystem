#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <cstring>
#include <iostream>
#include "vector.hpp"

template<int maxLen = 20>
class MyString {
public:
    int len;
    char str[maxLen];
    MyString();
    char& operator [](int);
    MyString& operator =(const char*);
    MyString& operator =(std::string);
    MyString& operator =(const MyString&);
    bool operator <(const MyString&);
    bool operator ==(const MyString&);
    friend std::ostream& operator <<(std::ostream& os, const MyString& myStr) {
        for (int i = 0; i < myStr.len; ++i) {
            os << myStr.str[i];
        }
        return os;
    }
};
/**
 * 由于编译器无法链接到 cpp 文件，不得不放在这里。
 * 不知道原因。
 */
template<int maxLen>
MyString<maxLen>::MyString() {
    len = 0;
}
template<int maxLen>
char& MyString<maxLen>::operator[] (int x) {
    return str[x];
}
template<int maxLen>
MyString<maxLen>& MyString<maxLen>::operator =(const char* other) {
    this->len = strlen(other);
    for (int i = 0; i < this->len; ++i) {
        this->str[i] = other[i];
    }
    return *this;
}
template<int maxLen>
MyString<maxLen>& MyString<maxLen>::operator =(std::string other) {
    this->len = other.length();
    for (int i = 0; i < this->len; ++i) {
        this->str[i] = other[i];
    }
    return *this;
}
template<int maxLen>
MyString<maxLen>& MyString<maxLen>::operator =(const MyString& other) {
    this->len = other.len;
    for (int i = 0; i < this->len; ++i) {
        this->str[i] = other.str[i];
    }
    return *this;
}
template<int maxLen>
bool MyString<maxLen>::operator <(const MyString& other) {
    int l;
    if (this->len < other.len) l = this->len;
    else l = other.len;
    for (int i = 0; i < l; ++i) {
        if (this->str[i] < other.str[i]) {
            return true;
        }
        if (this->str[i] > other.str[i]) {
            return false;
        }
    }
    if (this->len < other.len) return true;
    else return false;
}
template<int maxLen>
bool MyString<maxLen>::operator ==(const MyString& other) {
    if (this->len != other.len) return false;
    for (int i = 0; i < this->len; ++i) {
        if (this->str[i] != other.str[i]) {
            return false;
        }
    }
    return true;
}

template<class T1, class T2>
class Pair {
public:
    T1 first;
    T2 second;
    Pair() = default;
    Pair(T1 first, T2 second);
    bool operator <(const Pair& other);
    bool operator ==(const Pair& other);
    friend std::ostream& operator <<(std::ostream& os, const Pair& myPair) {
        return os;
    }
};
template<class T1, class T2>
Pair<T1, T2>::Pair(T1 first, T2 second) {
    this->first = first;
    this->second = second;
}
template<class T1, class T2>
bool Pair<T1, T2>::operator <(const Pair& other) {
    if (this->first == other.first) {
        return this->second < other.second;
    } else {
        return this->first < other.first;
    }
}
template<class T1, class T2>
bool Pair<T1, T2>::operator ==(const Pair& other) {
    return this->first == other.first && this->second == other.second;
}

using Date = Pair<int, int>;
using Time = Pair<int, int>;

int getNumDay(Date, Date);
int getDateInt(Date);
Date getDate(int);
std::string getDateString(int);
std::string getDateString(Date);
int getTimeInt(Time);
Time getTime(int);
std::string getTimeString(int);
std::string getTimeString(Time);

using Username = MyString<20>;
using Password = MyString<30>;
using Name = MyString<10>;
using MailAddress = MyString<30>;

class User {
public:
    Username username;
    Password password;
    Name name;
    MailAddress mailAddr;
    int privilege;
    User() = default;
    User(Username, Password, Name, MailAddress, int);
    bool operator <(const User&);
    bool operator ==(const User&);
};

constexpr int maxStation = 100;
using TrainID = MyString<20>;
using Station = MyString<10>;

class Train {
public:
    TrainID trainID;
    int stationNum;
    Station stations[maxStation];
    int seatNum;
    int prices[maxStation];
    Time startTime;
    int travelTimes[maxStation]; // 前缀和
    int stopoverTimes[maxStation]; // 前缀和
    Date saleStart, saleEnd;
    char type;
    friend class TrainManagement;
    friend class TicketManagement;
    Train() = default;
    Train(TrainID, int, Station[maxStation], int, int[maxStation], Time, int[maxStation], int[maxStation], Date, Date, char);
    Train& operator =(const Train&);
    bool operator <(const Train&);
    bool operator ==(const Train&);
};

enum Status {
    success, pending, refunded
};

class Order {
public:
    Status status;
    TrainID trainID;
    Station from, to;
    Date startingDate;
    Date leavingDate, arrivingDate;
    Time leavingTime, arrivingTime;
    int price;
    int num;
    Order() = default;
    Order(Status, TrainID, Station, Station, Date, Date, Date, Time, Time, int, int);
    bool operator <(const Order&);
    bool operator ==(const Order&);
};

constexpr int maxDate = 100;
class TicketInfo {
public:
    Train train;
    int dateLen;
    int seat[maxDate][maxStation];
    TicketInfo() = default;
    TicketInfo(const Train&);
    TicketInfo(const TicketInfo&);
    bool operator <(const TicketInfo&);
    bool operator ==(const TicketInfo&);
};

std::string get_token(std::string, int);
sjtu::vector<std::string> get_token_list(std::string);

#endif // UTILITY_HPP
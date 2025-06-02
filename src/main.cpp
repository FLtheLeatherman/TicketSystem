#include "ticket.hpp"
#include "train.hpp"
#include "user.hpp"

BPlusTree<Username, User, 100> bpt1;
BPlusTree<Username, bool, 100> bpt2;
BPlusTree<TrainID, Train, 50> bpt3;
BPlusTree<TrainID, int, 100> bpt5;
MemoryRiver<TicketInfo, 1> mr1;
BPlusTree<Pair<Station, Station>, int, 100> bpt6;
BPlusTree<Station, int, 100> bpt7;
BPlusTree<Username, int, 100> bpt8;
MemoryRiver<Order, 1> mr2;
BPlusTree<Pair<TrainID, Date>, int, 50> bpt9;
UserManagement user;
TrainManagement train;
TicketManagement ticket;
void clean() {
    user.clear();
    train.clear();
    ticket.clear();
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
int main() {
    user.initialize(&bpt1, &bpt2);
    train.initialize(&bpt3, &bpt5, &mr1, &bpt6, &bpt7);
    ticket.initialize(&bpt5, &mr1, &bpt8, &mr2, &bpt9);
    int timeStamp = 0;
    std::string command;
    while (getline(std::cin, command)) {
        sjtu::vector<std::string> tokens = get_token_list(command);
        if (tokens[0] == "add_user") {
            Username curUsername, username;
            Password password;
            Name name;
            MailAddress mailAddr;
            int privilege;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-p") password = tokens[i + 1];
                else if (tokens[i] == "-n") name = tokens[i + 1];
                else if (tokens[i] == "-m") mailAddr = tokens[i + 1];
                else if (tokens[i] == "-g") privilege = std::stoi(tokens[i + 1]);
            }
            user.add_user(curUsername, username, password, name, mailAddr, privilege);
        } else if (tokens[0] == "login") {
            Username username;
            Password password;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-p") password = tokens[i + 1];
            }
            user.login(username, password);
        } else if (tokens[0] == "logout") {
            Username username;
            username = tokens[2];
            user.logout(username);
        } else if (tokens[0] == "query_profile") {
            Username curUsername, username;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
            }
            user.query_profile(curUsername, username);
        } else if (tokens[0] == "modify_profile") {
            Username curUsername, username;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
            }
            user.modify_profile(curUsername, username, command);
        } else if (tokens[0] == "add_train") {
            TrainID trainID;
            int stationNum;
            int seatNum;
            std::string stations, prices, travelTimes, stopoverTimes;
            Time startTime;
            Date saleStart, saleEnd;
            char type;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-i") trainID = tokens[i + 1];
                else if (tokens[i] == "-n") stationNum = std::stoi(tokens[i + 1]);
                else if (tokens[i] == "-m") seatNum = std::stoi(tokens[i + 1]);
                else if (tokens[i] == "-s") stations = tokens[i + 1];
                else if (tokens[i] == "-p") prices = tokens[i + 1];
                else if (tokens[i] == "-x") startTime = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-t") travelTimes = tokens[i + 1];
                else if (tokens[i] == "-o") stopoverTimes = tokens[i + 1];
                else if (tokens[i] == "-y") type = tokens[i + 1][0];
                else if (tokens[i] == "-d") {
                    saleStart = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                    saleEnd = Pair<int, int>(std::stoi(tokens[i + 1].substr(6, 2)), std::stoi(tokens[i + 1].substr(9, 2)));
                }
            }
            train.add_train(trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleStart, saleEnd, type);
        } else if (tokens[0] == "delete_train") {
            TrainID trainID;
            trainID = tokens[2];
            train.delete_train(trainID);
        } else if (tokens[0] == "release_train") {
            TrainID trainID;
            trainID = tokens[2];
            train.release_train(trainID);
        } else if (tokens[0] == "query_train") {
            TrainID trainID;
            Date date;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-i") trainID = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
            }
            train.query_train(trainID, date);
        } else if (tokens[0] == "query_ticket") {
            Station from, to;
            Date date;
            bool flag = false;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-s") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-p") flag = (tokens[i + 1] == "cost");
            }
            train.query_ticket(from, to, date, flag);
        } else if (tokens[0] == "query_transfer") {
            Station from, to;
            Date date;
            bool flag = false;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-s") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-p") flag = (tokens[i + 1] == "cost");
            }
            train.query_transfer(from, to, date, flag);
        } else if (tokens[0] == "buy_ticket") {
            Username username;
            TrainID trainID;
            Date date;
            int num;
            Station from, to;
            bool flag = false;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-s") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-i") trainID = tokens[i + 1];
                else if (tokens[i] == "-n") num = std::stoi(tokens[i + 1]);
                else if (tokens[i] == "-q") flag = (tokens[i + 1] == "true");
            }
            ticket.buy_ticket(username, trainID, date, num, from, to, flag);
        } else if (tokens[0] == "query_order") {
            Username username;
            username = tokens[2];
            ticket.query_order(username);
        } else if (tokens[0] == "refund_ticket") {
            Username username;
            int num = 1;
            for (int i = 1; i < tokens.size(); i += 2) {
                if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-n") num = std::stoi(tokens[i + 1]);
            }
            ticket.refund_ticket(username, num);
        } else if (tokens[0] == "clean") {
            clean();
        } else if (tokens[0] == "exit") {
            break;
        }
    }
}
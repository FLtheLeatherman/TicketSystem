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
int main() {
    // freopen("a.in", "r", stdin);
    // freopen("a.ans", "w", stdout);
    user.initialize(&bpt1, &bpt2);
    train.initialize(&bpt3, &bpt5, &mr1, &bpt6, &bpt7);
    ticket.initialize(&bpt2, &bpt5, &mr1, &bpt8, &mr2, &bpt9);
    int timeStamp = 0;
    std::string command;
    while (getline(std::cin, command)) {
        // int sb;
        // mr1.get_info(sb, 1);
        // std::cout << "!!!!" << sb << std::endl;
        sjtu::vector<std::string> tokens = get_token_list(command);
        std::cout << tokens[0] << ' ';
        // int sbsb;
        // mr1.get_info(sbsb, 1);
        // std::cout << "!!!!" << sbsb << std::endl;
        if (tokens[1] == "add_user") {
            Username curUsername, username;
            Password password;
            Name name;
            MailAddress mailAddr;
            int privilege;
            // std::cout << command << '\n';
            for (int i = 2; i < tokens.size(); i += 2) {
                // std::cout << tokens[i] << ' ' << tokens[i + 1] << ' ';
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-p") password = tokens[i + 1];
                else if (tokens[i] == "-n") name = tokens[i + 1];
                else if (tokens[i] == "-m") mailAddr = tokens[i + 1];
                else if (tokens[i] == "-g") privilege = std::stoi(tokens[i + 1]);
            }
            // std::cout << '\n';
            // std::cout << name << '\n';
            user.add_user(curUsername, username, password, name, mailAddr, privilege);
        } else if (tokens[1] == "login") {
            Username username;
            Password password;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-p") password = tokens[i + 1];
            }
            // std::cout << "???\n";
            user.login(username, password);
            // std::cout << "!!!\n";
        } else if (tokens[1] == "logout") {
            Username username;
            username = tokens[3];
            user.logout(username);
        } else if (tokens[1] == "query_profile") {
            Username curUsername, username;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
            }
            user.query_profile(curUsername, username);
        } else if (tokens[1] == "modify_profile") {
            Username curUsername, username;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-c") curUsername = tokens[i + 1];
                else if (tokens[i] == "-u") username = tokens[i + 1];
            }
            user.modify_profile(curUsername, username, command);
        } else if (tokens[1] == "add_train") {
            TrainID trainID;
            int stationNum;
            int seatNum;
            std::string stations, prices, travelTimes, stopoverTimes;
            Time startTime;
            Date saleStart, saleEnd;
            char type;
            for (int i = 2; i < tokens.size(); i += 2) {
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
            // std::cout << "good" << std::endl;
            train.add_train(trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleStart, saleEnd, type);
        } else if (tokens[1] == "delete_train") {
            TrainID trainID;
            trainID = tokens[3];
            train.delete_train(trainID);
        } else if (tokens[1] == "release_train") {
            // int fuck;
            // mr1.get_info(fuck, 1);
            // std::cout << '?' << fuck << std::endl;
            TrainID trainID;
            trainID = tokens[3];
            // std::cout << trainID << std::endl;
            // int id;
            // mr1.get_info(id, 1);
            // std::cout << '?' << id << std::endl;
            train.release_train(trainID);
            // int tmp_id;
            // mr1.get_info(tmp_id, 1);
            // std::cout << '?' << tmp_id << std::endl;
            // TicketInfo tmp;
            // mr1.read(tmp, 1);
            // std::cout << tmp.train.trainID << std::endl;
        } else if (tokens[1] == "query_train") {
            TrainID trainID;
            Date date;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-i") trainID = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
            }
            train.query_train(trainID, date);
        } else if (tokens[1] == "query_ticket") {
            Station from, to;
            Date date;
            bool flag = false;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-s") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-p") flag = (tokens[i + 1] == "cost");
            }
            train.query_ticket(from, to, date, flag);
        } else if (tokens[1] == "query_transfer") {
            Station from, to;
            Date date;
            bool flag = false;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-s") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-p") flag = (tokens[i + 1] == "cost");
            }
            train.query_transfer(from, to, date, flag);
        } else if (tokens[1] == "buy_ticket") {
            Username username;
            TrainID trainID;
            Date date;
            int num;
            Station from, to;
            bool flag = false;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-f") from = tokens[i + 1];
                else if (tokens[i] == "-t") to = tokens[i + 1];
                else if (tokens[i] == "-d") date = Pair<int, int>(std::stoi(tokens[i + 1].substr(0, 2)), std::stoi(tokens[i + 1].substr(3, 2)));
                else if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-i") trainID = tokens[i + 1];
                else if (tokens[i] == "-n") num = std::stoi(tokens[i + 1]);
                else if (tokens[i] == "-q") flag = (tokens[i + 1] == "true");
            }
            ticket.buy_ticket(username, trainID, date, num, from, to, flag);
        } else if (tokens[1] == "query_order") {
            Username username;
            username = tokens[3];
            ticket.query_order(username);
        } else if (tokens[1] == "refund_ticket") {
            Username username;
            int num = 1;
            for (int i = 2; i < tokens.size(); i += 2) {
                if (tokens[i] == "-u") username = tokens[i + 1];
                else if (tokens[i] == "-n") num = std::stoi(tokens[i + 1]);
            }
            ticket.refund_ticket(username, num);
        } else if (tokens[1] == "clean") {
            // clean();
            bpt2.clear();
            std::cout << '\n';
        } else if (tokens[1] == "exit") {
            std::cout << "bye\n";
            break;
        }
        // TicketInfo tmp;
        // mr1.read(tmp, 2);
        // std::cout << tmp.train.trainID << std::endl;
        // if (tokens[0] == "[79373]") break;
        // std::cout.flush();
    }
}
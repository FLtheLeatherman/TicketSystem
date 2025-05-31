#include "user.hpp"

User::User(Username username, Password password, Name name, MailAddress mailAddr, Privilege privilege) {
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

UserManagement::~UserManagement() {
    bpt2.clear();
}
void UserManagement::clear() {
    bpt1.clear();
    bpt2.clear();
}
void UserManagement::initialize() {
    bpt1.initialise("user");
}
void UserManagement::add_user(Username curUser, Username username, Password password, Name name, MailAddress mailAddr, Privilege privilege) {
    User newUser = User(username, password, name, mailAddr, privilege);
    if (bpt1.is_empty()) {
        newUser.privilege = 10;
        bpt1.insert(username, newUser);
    } else {
        if (bpt2.contain(username)) {
            std::cout << -1 << '\n';
        } else {
            sjtu::vector<User> res = bpt1.show(username);
            User cur = res[0];
            if (cur.privilege > newUser.privilege) {
                bpt1.insert(username, newUser);
                std::cout << 0 << '\n';
            } else {
                std::cout << -1 << '\n';
            }
        }
    }
}
void UserManagement::login(Username username, Password password) {
    sjtu::vector<User> res1 = bpt1.show(username);
    if (res1.empty() || !(res1[0].password == password)) {
        std::cout << -1 << '\n';
        return;
    }
    if (!bpt2.contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    bpt2.insert(username, true);
    std::cout << 0 << '\n';
}
void UserManagement::logout(Username username) {
    if (!bpt2.contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    bpt2.erase(username, true);
    std::cout << 0 << '\n';
}
void UserManagement::query_profile(Username curUser, Username username) {
    sjtu::vector<User> res1 = bpt1.show(curUser);
    sjtu::vector<User> res2 = bpt1.show(username);
    if (!bpt2.contain(curUser) || res1.empty() || res2.empty() || res1[0].privilege < res2[0].privilege) {
        std::cout << -1 << '\n';
    }
    if (res1[0].privilege == res2[0].privilege && !(res1[0] == res2[0])) {
        std::cout << -1 << '\n';
    }
    std::cout << res2[0].username << ' ' << res2[0].name << ' ' << res2[0].mailAddr << ' ' << res2[0].privilege << '\n';
}
void UserManagement::modify_profile(Username curUser, Username username, std::string info) {
    sjtu::vector<User> res1 = bpt1.show(curUser);
    sjtu::vector<User> res2 = bpt1.show(username);
    if (!bpt2.contain(curUser) || res1.empty() || res2.empty() || res1[0].privilege < res2[0].privilege) {
        std::cout << -1 << '\n';
    }
    if (res1[0].privilege == res2[0].privilege && !(res1[0] == res2[0])) {
        std::cout << -1 << '\n';
    }
    bool flag = true;
    User newUser = res2[0];
    for (int i = 0; i < info.length(); ++i) {
        std::string tmp;
        int j = i + 3;
        while (j < info.length() && info[j] != ' ') {
            tmp += info[j];
            j++;
        }
        if (info[i + 1] == 'p') {
            newUser.password = tmp;
        } else if (info[i + 1] == 'n') {
            newUser.name = tmp;
        } else if (info[i + 1] == 'm') {
            newUser.mailAddr = tmp;
        } else {
            Privilege now = std::stoi(tmp);
            if (now >= res1[0].privilege) {
                flag = false;
                break;
            }
            newUser.privilege = now;
        }
    }
    if (flag) {
        bpt1.erase(username, res2[0]);
        bpt1.insert(username, newUser);
    }
}
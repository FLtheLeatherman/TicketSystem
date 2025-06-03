#include "user.hpp"

UserManagement::~UserManagement() {
    bpt2->clear();
}
void UserManagement::clear() {
    bpt1->clear();
    bpt2->clear();
}
void UserManagement::initialize(BPlusTree<Username, User, 100>* bpt1, BPlusTree<Username, bool, 100>* bpt2) {
    this->bpt1 = bpt1;
    this->bpt2 = bpt2;
    bpt1->initialize("user");
    bpt2->initialize("user_stack");
}
void UserManagement::add_user(Username curUser, Username username, Password password, Name name, MailAddress mailAddr, int privilege) {
    // std::cout << name << '\n';
    User newUser = User(username, password, name, mailAddr, privilege);
    if (bpt1->is_empty()) {
        newUser.privilege = 10;
        // std::cout << newUser.name << '\n';
        bpt1->insert(username, newUser);
        std::cout << 0 << '\n';
    } else {
        if (bpt1->contain(username) || !bpt2->contain(curUser)) {
            std::cout << -1 << '\n';
        } else {
            sjtu::vector<User> res = bpt1->show(curUser);
            User cur = res[0];
            if (cur.privilege > newUser.privilege) {
                bpt1->insert(username, newUser);
                std::cout << 0 << '\n';
            } else {
                std::cout << -1 << '\n';
            }
        }
    }
}
void UserManagement::login(Username username, Password password) {
    sjtu::vector<User> res1 = bpt1->show(username);
    // std::cout << res1.size() << '\n';
    // std::cout << res1[0].name << '\n';
    if (res1.empty() || !(res1[0].password == password)) {
        std::cout << -1 << '\n';
        return;
    }
    if (bpt2->contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    bpt2->insert(username, true);
    std::cout << 0 << '\n';
}
void UserManagement::logout(Username username) {
    if (!bpt2->contain(username)) {
        std::cout << -1 << '\n';
        return;
    }
    bpt2->erase(username, true);
    std::cout << 0 << '\n';
}
void UserManagement::query_profile(Username curUser, Username username) {
    sjtu::vector<User> res1 = bpt1->show(curUser);
    sjtu::vector<User> res2 = bpt1->show(username);
    if (!bpt2->contain(curUser) || res1.empty() || res2.empty() || res1[0].privilege < res2[0].privilege) {
        std::cout << -1 << '\n';
        return;
    }
    if (res1[0].privilege == res2[0].privilege && !(res1[0] == res2[0])) {
        std::cout << -1 << '\n';
        return;
    }
    std::cout << res2[0].username << ' ' << res2[0].name << ' ' << res2[0].mailAddr << ' ' << res2[0].privilege << '\n';
}
void UserManagement::modify_profile(Username curUser, Username username, std::string info) {
    sjtu::vector<User> res1 = bpt1->show(curUser);
    sjtu::vector<User> res2 = bpt1->show(username);
    if (!bpt2->contain(curUser) || res1.empty() || res2.empty() || res1[0].privilege < res2[0].privilege) {
        std::cout << -1 << '\n';
        return;
    }
    if (res1[0].privilege == res2[0].privilege && !(res1[0] == res2[0])) {
        std::cout << -1 << '\n';
        return;
    }
    bool flag = true;
    sjtu::vector<std::string> tokens = get_token_list(info);
    User newUser = res2[0];
    for (int i = 2; i < tokens.size(); i += 2) {
        if (tokens[i] == "-p") newUser.password= tokens[i + 1];
        else if (tokens[i] == "-n") newUser.name = tokens[i + 1];
        else if (tokens[i] == "-m") newUser.mailAddr = tokens[i + 1];
        else if (tokens[i] == "-g") {
            newUser.privilege = std::stoi(tokens[i + 1]);
            if (newUser.privilege >= res1[0].privilege) {
                std::cout << -1 << '\n';
                return;
            }
        }
    }
    if (flag) {
        bpt1->erase(username, res2[0]);
        bpt1->insert(username, newUser);
    }
    std::cout << newUser.username << ' ' << newUser.name << ' ' << newUser.mailAddr << ' ' << newUser.privilege << '\n';
}
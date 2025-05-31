#ifndef USER_HPP
#define USER_HPP

#include "bpt.hpp"
#include "utility.hpp"
#include <string>
#include <cstring>

class UserManagement {
private:
    BPlusTree<Username, User, 100>* bpt1; // 所有用户
    BPlusTree<Username, bool, 100>* bpt2; // 登录中的用户
public:
    UserManagement() = default;
    ~UserManagement();
    void initialize();
    void clear();
    void add_user(Username, Username, Password, Name, MailAddress, int);
    void login(Username, Password);
    void logout(Username);
    void query_profile(Username, Username);
    void modify_profile(Username, Username, std::string);
};

#endif // USER_HPP
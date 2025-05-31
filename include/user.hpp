#ifndef USER_HPP
#define USER_HPP

#include "bpt.hpp"
#include "utility.hpp"
#include <string>
#include <cstring>

using Username = MyString<20>;
using Password = MyString<30>;
using Name = MyString<5>;
using MailAddress = MyString<30>;
using Privilege = int;

class User {
private:
    Username username;
    Password password;
    Name name;
    MailAddress mailAddr;
    Privilege privilege;
    friend class UserManagement;
public:
    User() = default;
    User(Username, Password, Name, MailAddress, Privilege);
    bool operator <(const User&);
    bool operator ==(const User&);
};

class UserManagement {
private:
    BPlusTree<Username, User, 100> bpt1; // 所有用户
    BPlusTree<Username, bool, 100> bpt2; // 登录中的用户
public:
    UserManagement() = default;
    ~UserManagement() = default;
    void initialize();
    void add_user(Username, Username, Password, Name, MailAddress, Privilege);
    void login(Username, Password);
    void logout(Username);
    void query_profile(Username, Username);
    void modify_profile(Username, Username, std::string);
};

#endif // USER_HPP
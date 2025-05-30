#ifndef USER_HPP
#define USER_HPP

#include "bpt.hpp"
#include "utility.hpp"

using Username = MyString<20>;
using Password = MyString<30>;
using Name = MyString<5>;
using MailAddress = MyString<30>;

class User {
private:
    Username username;
    Password password;
    Name name;
    MailAddress mailAddr;
    int privilege;
public:
    User() = default;
    bool operator <(const User&);
    bool operator ==(const User&);
};

class UserManagement {
private:
    BPlusTree<Username, User, 100> bpt1;
    sjtu::vector<Username> currentUser;
public:
    UserManagement() = default;
    ~UserManagement() = default;
    void initialize();
    void add_user();
    void login();
    void logout();
    void query_profile();
    void modify_profile();
};

#endif // USER_HPP
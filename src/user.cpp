#include "user.hpp"

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
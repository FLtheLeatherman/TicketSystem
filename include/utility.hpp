#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <cstring>
#include <iostream>

template<int maxLen = 20>
class MyString {
private:
    int len;
    char str[maxLen];
public:
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

class Train {

};

#endif // UTILITY_HPP
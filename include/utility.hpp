#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstring>

template<int maxLen = 20>
class MyString {
private:
    int len;
    char str[maxLen];
public:
    MyString();
    char& operator [](int);
    MyString& operator =(const char*);
    MyString& operator =(const MyString&);
    bool operator <(const MyString&);
    bool operator ==(const MyString&);
};
/**
 * 由于编译器无法链接到 cpp 文件，不得不放在这里。
 * 不知道原因。
 */
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
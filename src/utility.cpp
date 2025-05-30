#include "utility.hpp"

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
}
template<int maxLen>
MyString<maxLen>& MyString<maxLen>::operator =(const MyString& other) {
    this->len = other.len;
    for (int i = 0; i < this->len; ++i) {
        this->str[i] = other.str[i];
    }
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

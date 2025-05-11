#include "BPT.hpp"
#include <iostream>
#include <string>
constexpr int base = 19260817;
constexpr int mod = 998244353;
int getHash(std::string str) {
    int res = 0, len = str.length();
    for (int i = 0; i < len; ++i) {
        res = 1ll * res * base % mod + str[i];
    }
    return res;
}
BPlusTree<int, int, 300> bpt;
void insert(int key, int value) {

}
void myDelete(int key, int value) {

}
void find(int key) {

}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    while (n--) {
        std::string option, index;
        std::cin >> option >> index;
        int key = getHash(index);
        if (option == "insert") {
            int value;
            std::cin >> value;
            insert(key, value);
        } else if (option == "delete") {
            int value;
            std::cin >> value;
            myDelete(key, value);
        } else {
            find(key);
        }
    }
}
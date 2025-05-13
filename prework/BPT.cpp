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
BPlusTree<int, int, 4> bpt;
void insert(int key, int value) {
    bpt.insert(key, value);
}
void myDelete(int key, int value) {
    bpt.myDelete(key, value);
}
void find(int key) {
    bpt.show(key);
}
void debug() {
    bpt.traverse();
}
int main() {
    freopen("test", "r", stdin);
    freopen("test.out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    bpt.initialise("1");
    int n;
    std::cin >> n;
    while (n--) {
        std::string option, index;
        std::cin >> option >> index;
        int key = getHash(index);
        // std::cout << key << std::endl;
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
        // std::cout.flush();
        // if (n <= 20) {
        //     std::cout << "!!!" << n << std::endl;
        //     debug();
        //     std::cout << std::endl;
        // }
        // std::cout << n << std::endl;
        // exit(0);
        // std::cout << "!!!" << n << std::endl;
        // debug();
        // bpt.getList();
        // std::cout << std::endl;
    }
    // debug();
}
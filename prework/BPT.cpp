#include "BPT.hpp"
#include <iostream>
#include <string>
constexpr unsigned long long base = 21788233;
unsigned long long getHash(std::string str) {
    unsigned long long res = 0;
    int len = str.length();
    for (int i = 0; i < len; ++i) {
        res = res * base + (unsigned long long)str[i];
    }
    return res;
}
BPlusTree<unsigned long long, int, 115> bpt;
void insert(unsigned long long key, int value) {
    bpt.insert(key, value);
}
void myDelete(unsigned long long key, int value) {
    bpt.myDelete(key, value);
}
void find(unsigned long long key) {
    bpt.show(key);
}
void debug() {
    bpt.traverse();
    bpt.getList();
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    bpt.initialise("1");
    int n;
    std::cin >> n;
    while (n--) {
        std::string option, index;
        std::cin >> option >> index;
        unsigned long long key = getHash(index);
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
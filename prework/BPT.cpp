#include "BPT.hpp"
#include <iostream>
#include <string>
constexpr int base1 = 19260817, base2 = 1009;
constexpr int mod1 = 998244353, mod2 = 1e9 + 7;
unsigned long long getHash(std::string str) {
    int res1 = 0, res2 = 0, len = str.length();
    for (int i = 0; i < len; ++i) {
        res1 = 1ll * res1 * base1 % mod1 + str[i];
        res2 = 1ll * res2 * base2 % mod2 + str[i];
    }
    return (unsigned long long)res1 + (unsigned long long)res2 * 998244353ull;
}
BPlusTree<unsigned long long, int, 127> bpt;
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
    // freopen("test", "r", stdin);
    // freopen("test.out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    bpt.initialise("1");
    int n;
    std::cin >> n;
    while (n--) {
        std::string option, index;
        std::cin >> option >> index;
        unsigned long long key = getHash(index);
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
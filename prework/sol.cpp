#include <bits/stdc++.h>
int tot;
std::map<std::string, int> mp;
std::vector<int> vec[100010];
int main() {
    freopen("test.in", "r", stdin);
    freopen("test.ans", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    while (n--) {
        std::string option, index;
        std::cin >> option >> index;
        if (option == "insert") {
            int value;
            std::cin >> value;
            if (!mp[index]) mp[index] = ++tot;
            int id = mp[index];
            bool flag = true;
            for (int i = 0; i < vec[id].size(); ++i) {
                if (vec[id][i] == value) {
                    flag = false;
                    break;
                }
            }
            if (flag) vec[mp[index]].push_back(value);
        } else if (option == "delete") {
            int value;
            std::cin >> value;
            int id = mp[index];
            int pos = -1;
            for (int i = 0; i < vec[id].size(); ++i) {
                if (vec[id][i] == value) {
                    pos = i;
                    break;
                }
            }
            if (pos >= 0) vec[id].erase(vec[id].begin() + pos);
        } else {
            int id = mp[index];
            sort(vec[id].begin(), vec[id].end());
            if (vec[id].size() == 0) std::cout << "null";
            for (int x: vec[id]) {
                std::cout << x << ' ';
            }
            std::cout << '\n';
        }
    }
}
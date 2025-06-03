#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <iostream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<typename T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    // int tot;
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialize(string FN = "") {
        sizeofT = sizeof(T);
        // std::cerr << "!!" << FN << ' ' << info_len << ' ' << sizeofT << std::endl;
        file_name = FN;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            file.close();
            // std::cerr << FN << std::endl;
            file.open(file_name, std::ios::out);
            // tot = 0;
            for (size_t i = 0; i < info_len; ++i) {
                file.seekp(i * sizeof(int));
                int tmp = 0;
                file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
            }
        }
        file.close();
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
    }

    void clear() {
        for (size_t i = 0; i < info_len; ++i) {
            file.seekp(i * sizeof(int));
            int tmp = 0;
            file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        }
    }

    void get_info(int &tmp, int n) {
        // if (file_name == "ticket_info_") std::cout << "get_info!\n";
        if (n > info_len) return;
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
    }

    void write_info(int tmp, int n) {
        // if (file_name == "ticket_info_") std::cout << "write_info!" << tmp << "\n";
        // tot = std::max(tot, tmp);
        if (n > info_len) return;
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
    }

    void read(T &t, int index) {
        // if (file_name == "ticket_info_") std::cout << "read!" << index << '\n';
        file.seekg(info_len * sizeof(int) + (index - 1) * sizeofT);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
    }

    void write(T &t, int index) {
        // if (file_name == "ticket_info_") std::cout << "write!" << index << '\n';
        file.seekp(info_len * sizeof(int) + (index - 1) * sizeofT);
        file.write(reinterpret_cast<char*>(&t), sizeofT);
    }

    ~MemoryRiver() {
        file.close();
    }

};
#endif // BPT_MEMORYRIVER_HPP
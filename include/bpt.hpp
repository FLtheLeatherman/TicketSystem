#ifndef BPT_HPP
#define BPT_HPP

#include "memory_river.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "my_vector.hpp"

/**
 * 需要修改成：仅按照 T1 排序，而且越晚插进来的放越后面.
 * 那么只需让 T2 的 < 始终返回 false.
 *
 * T1 和 T2 需有 < 和 ==.
 */
template<class T1, class T2, int M> // 一个普通节点有 M (= L) 个 key, M + 1 个 children
class BPlusTree {
private:
    int tot; // 节点总数
    int root; // 根节点的位置
    struct info {
        T1 key;
        T2 val;
        info() {}
        info(T1 key, T2 val) {
            this->key = key;
            this->val = val;
        }
        info& operator =(const info& other) {
            key = other.key;
            val = other.val;
            return *this;
        }
        bool operator <(const info &other) {
            if (key != other.key) {
                return key < other.key;
            } else {
                return val < other.val;
            }
        }
        bool operator ==(const info &other) {
            return key == other.key && val == other.val;
        }
    };
    struct infoArr {
        info a[M + 1]; // 预留一位便于操作
    };
    MemoryRiver<infoArr, 1> information;
    struct node {
        bool isLeaf; // 是否叶子
        size_t size; // 目前的 key 数量
        int key; // key 在 information 文件里的位置
        int children[M + 2]; // children 在 bpt 文件里的位置；同样预留一位便于操作
        int prev, next; // leaf 链表在文件里的位置
        node() {
            isLeaf = false;
            size = 0;
            key = prev = next = -1;
        }
        node& operator =(const node &other) {
            isLeaf = other.isLeaf;
            size = other.size;
            key = other.key;
            for (int i = 0; i <= M; ++i) {
                children[i] = other.children[i];
            }
            prev = other.prev, next = other.next;
            return *this;
        }
        void print() {
            std::cout << isLeaf << ' ' << size << ' ' << key << ' ' << prev << ' ' << next << '\n';
            for (int i = 0; i <= size; ++i) {
                std::cout << children[i] << ' ';
            }
            std::cout << std::endl;
        }
    };
    MemoryRiver<node, 1> bpt;
    int top;
    node stack[100];
public:
    ~BPlusTree() {
        information.write_info(tot, 1);
        bpt.write_info(root, 1);
    }
    void initialise(std::string filename) {
        std::string fn1 = filename + "_info", fn2 = filename + "_bpt";
        information.initialize(fn1);
        bpt.initialize(fn2);
        information.get_info(tot, 1);
        bpt.get_info(root, 1);
    }
    /**
     * @return 需要继续分裂上层节点返回 true，否则返回 false
     */
    bool insert_upper(node curNode, bool &flag, info &newKey1, int &newPos1, info &newKey2, int &newPos2, info &lastKey) {
        infoArr curArr;
        int pos = curNode.size - 1;
        information.read(curArr, curNode.key);
        for (int i = 0; i < curNode.size; ++i) {
            if (lastKey < curArr.a[i]) {
                pos = i - 1;
                break;
            }
        }
        lastKey = curArr.a[0];
        for (int i = curNode.size; i > pos + 1; --i) {
            curArr.a[i] = curArr.a[i - 1];
        }
        if (flag && pos >= 0) curArr.a[pos] = newKey1;
        curArr.a[pos + 1] = newKey2;
        for (int i = curNode.size + 1; i > pos + 2; --i) {
            curNode.children[i] = curNode.children[i - 1];
        }
        curNode.children[pos + 1] = newPos1, curNode.children[pos + 2] = newPos2;
        if (curNode.size < M) {
            curNode.size++;
            information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
            return false;
        } else {
            curNode.size = (M + 1) / 2; 
            node newNode;
            infoArr newArr;
            newNode.size = M / 2, newNode.key = ++tot;
            for (int i = curNode.size + 1; i <= M; ++i) {
                newArr.a[i - curNode.size - 1] = curArr.a[i];
            }
            for (int i = curNode.size + 1; i <= M + 1; ++i) {
                newNode.children[i - curNode.size - 1] = curNode.children[i];
            }
            if (pos >= 0) {
                flag = false;
            }
            newKey2 = curArr.a[curNode.size];
            newPos1 = curNode.key, newPos2 = tot;
            information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
            information.write(newArr, tot), bpt.write(newNode, tot);
            return true;
        }
    }
    void insert(T1 key, T2 val) {
        info curInfo = info(key, val);
        if (!root) {
            node curNode;
            infoArr curArr;
            curNode.isLeaf = 1, curNode.size = 1;
            curNode.key = ++tot;
            curArr.a[0] = curInfo;
            information.write(curArr, tot);
            bpt.write(curNode, tot);
            node rootNode;
            infoArr rootArr;
            rootNode.size = 0, rootNode.key = ++tot;
            rootNode.children[0] = tot - 1;
            root = tot;
            information.write(rootArr, tot);
            bpt.write(rootNode, tot);
        } else {
            infoArr curArr;
            node curNode;
            bpt.read(curNode, root);
            top = 0;
            while (!curNode.isLeaf) {
                stack[top++] = curNode;
                int pos = curNode.size;
                information.read(curArr, curNode.key);
                for (int i = 0; i < curNode.size; ++i) {
                    if (curInfo < curArr.a[i]) {
                        pos = i;
                        break;
                    }
                }
                bpt.read(curNode, curNode.children[pos]);
            }
            information.read(curArr, curNode.key);
            for (int i = 0; i < curNode.size; ++i) {
                if (curInfo == curArr.a[i]) {
                    return;
                }
            }
            int pos = curNode.size;
            for (int i = 0; i < curNode.size; ++i) {
                if (curInfo < curArr.a[i]) {
                    pos = i;
                    break;
                }
            }
            info lastKey = curArr.a[0];
            for (int i = curNode.size; i > pos; --i) {
                curArr.a[i] = curArr.a[i - 1];
            }
            curArr.a[pos] = curInfo;
            if (curNode.size < M) {
                curNode.size++;
                information.write(curArr, curNode.key);
                bpt.write(curNode, curNode.key);
            } else {
                curNode.size = M / 2 + 1;
                node newNode;
                infoArr newArr;
                newNode.isLeaf = true, newNode.size = (M + 1) / 2, newNode.key = ++tot;
                newNode.next = curNode.next, newNode.prev = curNode.key;
                if (curNode.next != -1) {
                    node nodeNext;
                    bpt.read(nodeNext, curNode.next);
                    nodeNext.prev = tot;
                    bpt.write(nodeNext, curNode.next);
                }
                curNode.next = tot;
                for (int i = curNode.size; i <= M; ++i) {
                    newArr.a[i - curNode.size] = curArr.a[i];
                }
                information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                information.write(newArr, tot), bpt.write(newNode, tot);
                bool flag = true;
                info newKey1 = curArr.a[0], newKey2 = newArr.a[0];
                int newPos1 = curNode.key, newPos2 = tot;
                for (int i = top - 1; i >= 0; --i) {
                    if (!insert_upper(stack[i], flag, newKey1, newPos1, newKey2, newPos2, lastKey)) {
                        break;
                    }
                    if (i == 0) {
                        node rootNode;
                        infoArr rootArr;
                        rootNode.size = 1;
                        rootNode.children[0] = newPos1, rootNode.children[1] = newPos2;
                        rootNode.key = ++tot;
                        rootArr.a[0] = newKey2;
                        root = tot;
                        information.write(rootArr, tot), bpt.write(rootNode, tot);
                    }
                }
            }
        }
    }
    /**
     * @return 需要继续合并上层节点返回 true，否则返回 false
     */
    bool delete_upper(node curNode, info &lastKey) {
        infoArr curArr;
        information.read(curArr, curNode.key);
        int pos = curNode.size;
        for (int i = 0; i < curNode.size; ++i) {
            if (lastKey < curArr.a[i]) {
                pos = i;
                break;
            }
        }
        node leftNode, rightNode;
        infoArr leftArr, rightArr;
        if (pos == 0) {
            bpt.read(leftNode, curNode.children[0]), bpt.read(rightNode, curNode.children[1]);
            information.read(leftArr, leftNode.key), information.read(rightArr, rightNode.key);
            if (rightNode.size + 1 > (M + 1) / 2) {
                leftArr.a[leftNode.size] = curArr.a[0];
                leftNode.children[leftNode.size + 1] = rightNode.children[0];
                curArr.a[0] = rightArr.a[0];
                for (int i = 0; i < rightNode.size - 1; ++i) {
                    rightArr.a[i] = rightArr.a[i + 1];
                }
                for (int i = 0; i < rightNode.size; ++i) {
                    rightNode.children[i] = rightNode.children[i + 1];
                }
                leftNode.size++;
                rightNode.size--;
                information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                information.write(rightArr, rightNode.key), bpt.write(rightNode, rightNode.key);
                return false;
            } else {
                leftArr.a[leftNode.size] = curArr.a[0];
                for (int i = leftNode.size + 1; i <= leftNode.size + rightNode.size; ++i) {
                    leftArr.a[i] = rightArr.a[i - leftNode.size - 1];
                }
                for (int i = leftNode.size + 1; i <= leftNode.size + rightNode.size + 1; ++i) {
                    leftNode.children[i] = rightNode.children[i - leftNode.size - 1];
                }
                for (int i = 0; i < curNode.size - 1; ++i) {
                    curArr.a[i] = curArr.a[i + 1];
                }
                for (int i = 1; i < curNode.size; ++i) {
                    curNode.children[i] = curNode.children[i + 1];
                }
                curNode.size--;
                leftNode.size += rightNode.size + 1;
                information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                if (curNode.size + 1 < (M + 1) / 2) return true;
                else return false;
            }
        } else {
            bpt.read(rightNode, curNode.children[pos]), bpt.read(leftNode, curNode.children[pos - 1]);
            information.read(rightArr, rightNode.key), information.read(leftArr, leftNode.key);
            if (leftNode.size + 1 > (M + 1) / 2) {
                for (int i = rightNode.size; i >= 1; --i) {
                    rightArr.a[i] = rightArr.a[i - 1];
                }
                for (int i = rightNode.size + 1; i >= 1; --i) {
                    rightNode.children[i] = rightNode.children[i - 1];
                }
                rightArr.a[0] = curArr.a[pos - 1];
                rightNode.children[0] = leftNode.children[leftNode.size];
                curArr.a[pos - 1] = leftArr.a[leftNode.size - 1];
                leftNode.size--;
                rightNode.size++;
                information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                information.write(rightArr, rightNode.key), bpt.write(rightNode, rightNode.key);
                information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                return false;
            } else {
                leftArr.a[leftNode.size] = curArr.a[pos - 1];
                for (int i = leftNode.size + 1; i <= rightNode.size + leftNode.size; ++i) {
                    leftArr.a[i] = rightArr.a[i - leftNode.size - 1];
                }
                for (int i = leftNode.size + 1; i <= rightNode.size + leftNode.size + 1; ++i) {
                    leftNode.children[i] = rightNode.children[i - leftNode.size - 1];
                }
                for (int i = pos - 1; i < curNode.size - 1; ++i) {
                    curArr.a[i] = curArr.a[i + 1];
                }
                for (int i = pos; i < curNode.size; ++i) {
                    curNode.children[i] = curNode.children[i + 1];
                }
                curNode.size--;
                leftNode.size += rightNode.size + 1;
                information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                if (curNode.size + 1 < (M + 1) / 2) return true;
                else return false;
            }
        }
    }
    void myDelete(T1 key, T2 val) {
        info curInfo = info(key, val);
        node curNode;
        infoArr curArr;
        if (!root) {
            return;
        } else {
            bpt.read(curNode, root);
            bool rootSizeIsZero = curNode.size == 0;
            top = 0;
            while (!curNode.isLeaf) {
                stack[top++] = curNode;
                information.read(curArr, curNode.key);
                int pos = curNode.size;
                for (int i = 0; i < curNode.size; ++i) {
                    if (curInfo < curArr.a[i]) {
                        pos = i;
                        break;
                    }
                }
                bpt.read(curNode, curNode.children[pos]);
            }
            information.read(curArr, curNode.key);
            int pos = curNode.size;
            for (int i = 0; i < curNode.size; ++i) {
                if (curInfo == curArr.a[i]) {
                    pos = i;
                    break;
                }
            }
            if (pos == curNode.size) return;
            if (pos == 0) {
                info lastKey = curArr.a[0];
                infoArr tmp;
                for (int i = top - 1; i >= 0; --i) {
                    information.read(tmp, stack[i].key);
                    for (int j = 0; j < stack[i].size; ++j) {
                        if (tmp.a[j] == lastKey) {
                            tmp.a[j] = curArr.a[1];
                        }
                    }
                    information.write(tmp, stack[i].key);
                }
            }
            for (int i = pos; i < curNode.size - 1; ++i) {
                curArr.a[i] = curArr.a[i + 1];
            }
            information.write(curArr, curNode.key);
            curNode.size--;
            bpt.write(curNode, curNode.key);
            if (rootSizeIsZero) {
                if (curNode.size == 0) {
                    root = 0;
                }
            } else if (curNode.size < (M + 1) / 2) {
                info lastKey = curArr.a[0];
                node faNode = stack[top - 1];
                infoArr faArr;
                information.read(faArr, faNode.key);
                int pos = -1;
                for (int i = 0; i < faNode.size; ++i) {
                    if (faArr.a[i] == lastKey) {
                        pos = i;
                        break;
                    }
                }
                if (pos >= 0) {
                    node leftNode;
                    infoArr leftArr;
                    bpt.read(leftNode, faNode.children[pos]);
                    information.read(leftArr, leftNode.key);
                    if (leftNode.size > (M + 1) / 2) {
                        for (int i = curNode.size; i >= 1; --i) {
                            curArr.a[i] = curArr.a[i - 1];
                        }
                        curArr.a[0] = leftArr.a[leftNode.size - 1];
                        curNode.size++;
                        leftNode.size--;
                        faArr.a[pos] = curArr.a[0];
                        information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                        information.write(faArr, faNode.key), bpt.write(faNode, faNode.key);
                        information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                    } else {
                        for (int i = leftNode.size; i < leftNode.size + curNode.size; ++i) {
                            leftArr.a[i] = curArr.a[i - leftNode.size];
                        }
                        leftNode.size += curNode.size;
                        if (curNode.next != -1) {
                            node nodeNext;
                            bpt.read(nodeNext, curNode.next);
                            nodeNext.prev = curNode.prev;
                            bpt.write(nodeNext, nodeNext.key);
                        }
                        leftNode.next = curNode.next;
                        for (int i = pos; i < faNode.size - 1; ++i) {
                            faArr.a[i] = faArr.a[i + 1];
                        }
                        for (int i = pos + 1; i < faNode.size; ++i) {
                            faNode.children[i] = faNode.children[i + 1];
                        }
                        faNode.size--;
                        information.write(faArr, faNode.key), bpt.write(faNode, faNode.key);
                        information.write(leftArr, leftNode.key), bpt.write(leftNode, leftNode.key);
                    }
                } else {
                    node rightNode;
                    infoArr rightArr;
                    bpt.read(rightNode, faNode.children[1]);
                    information.read(rightArr, rightNode.key);
                    if (rightNode.size > (M + 1) / 2) {
                        curArr.a[curNode.size] = rightArr.a[0];
                        for (int i = 0; i < rightNode.size - 1; ++i) {
                            rightArr.a[i] = rightArr.a[i + 1];
                        }
                        curNode.size++;
                        rightNode.size--;
                        faArr.a[0] = rightArr.a[0];
                        information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                        information.write(faArr, faNode.key), bpt.write(faNode, faNode.key);
                        information.write(rightArr, rightNode.key), bpt.write(rightNode, rightNode.key);
                    } else {
                        for (int i = curNode.size; i < rightNode.size + curNode.size; ++i) {
                            curArr.a[i] = rightArr.a[i - curNode.size];
                        }
                        curNode.size += rightNode.size;
                        if (rightNode.next != -1) {
                            node nodeNext;
                            bpt.read(nodeNext, rightNode.next);
                            nodeNext.prev = rightNode.prev;
                            bpt.write(nodeNext, nodeNext.key);
                        }
                        curNode.next = rightNode.next;
                        for (int i = 0; i < faNode.size - 1; ++i) {
                            faArr.a[i] = faArr.a[i + 1];
                        }
                        for (int i = 1; i < faNode.size; ++i) {
                            faNode.children[i] = faNode.children[i + 1];
                        }
                        faNode.size--;
                        information.write(faArr, faNode.key), bpt.write(faNode, faNode.key);
                        information.write(curArr, curNode.key), bpt.write(curNode, curNode.key);
                    }
                }
                if (faNode.size < (M + 1) / 2) {
                    lastKey = faArr.a[0];
                    for (int i = top - 2; i >= 0; --i) {
                        if (!delete_upper(stack[i], lastKey)) {
                            break;
                        }
                        if (i == 0) {
                            node rootNode;
                            bpt.read(rootNode, root);
                            if (rootNode.size == 0) {
                                root = rootNode.children[0];
                            }
                        }
                    }
                }
            }
        }
    }
    sjtu::vector<T2> show(T1 key) {
        sjtu::vector<T2> res;
        if (root == 0) {
            return res;
        }
        info curInfo = info(key, -2147483648);
        infoArr curArr;
        node curNode;
        bpt.read(curNode, root);
        while (!curNode.isLeaf) {
            information.read(curArr, curNode.key);
            int pos = curNode.size;
            for (int i = 0; i < curNode.size; ++i) {
                if (curInfo < curArr.a[i]) {
                    pos = i;
                    break;
                }
            }
            bpt.read(curNode, curNode.children[pos]);
        }
        bool flag = true;
        while (flag) {
            information.read(curArr, curNode.key);
            for (int i = 0; i < curNode.size; ++i) {
                if (curArr.a[i].key == key) {
                    res.push_back(curArr.a[i].val);
                } else if (key < curArr.a[i].key) {
                    flag = false;
                    break;
                }
            }
            if (curNode.next == -1) {
                flag = false;
            } else {
                bpt.read(curNode, curNode.next);
            }
        }
        return res;
    }
    void traverse(int id = -1) {
        if (root == 0) return;
        if (id == -1) id = root;
        node curNode;
        infoArr curArr;
        bpt.read(curNode, id);
        information.read(curArr, curNode.key);
        std::cout << '!' << curNode.key << ' ' << curNode.isLeaf << ":\n";
        for (int i = 0; i < curNode.size; ++i) {
            std::cout << curArr.a[i].key << ' ' << curArr.a[i].val << '|';
        } 
        std::cout << '\n';
        if (curNode.isLeaf) return;
        for (int i = 0; i <= curNode.size; ++i) {
            std::cout << curNode.children[i] << ' ';
        }
        std::cout << '\n';
        for (int i = 0; i <= curNode.size; ++i) {
            traverse(curNode.children[i]);
        }
    }
    void getList() {
        node curNode;
        bpt.read(curNode, root);
        while (!curNode.isLeaf) {
            bpt.read(curNode, curNode.children[0]);
        }
        while (1) {
            infoArr curArr;
            information.read(curArr, curNode.key);
            for (int i = 0; i < curNode.size; ++i) {
                std::cout << curArr.a[i].key << ' ';
            }
            std::cout << std::endl;
            if (curNode.next == -1) break;
            else bpt.read(curNode, curNode.next);
        }
    }
};

#endif // BPT_HPP
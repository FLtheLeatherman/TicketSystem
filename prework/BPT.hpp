#include "MemoryRiver.hpp"
#include <fstream>
#include <string>

template<class T1, class T2, int M> // 一个普通节点有 M (= L) 个 key, M + 1 个 children
class BPlusTree {
private:
    int tot; // 节点总数；可以上垃圾回收 1-based
    int root; // 根节点的位置
    struct info {
        T1 key;
        T2 val;
        info() {}
        info(T1 key, T2 val) {
            key = key;
            val = val;
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
        info a[M + 1]; // 预留一位便于神秘操作
    };
    MemoryRiver<infoArr, 1> information;
    struct node {
        bool isLeaf; // 是否叶子
        size_t size; // 目前的 key 数量
        int key; // key 在 info 文件里的位置
        int children[M + 2]; // children 在文件里的位置；同样预留一位便于操作
        int prev, next; // leaf 链表在文件里的位置
        node() {
            isLeaf = false;
            size = -1;
            key = prev = next = -1;
        }
        node operator =(const node &other) {
            isLeaf = other.isLeaf;
            size = other.siez;
            key = other.key;
            for (int i = 0; i <= M; ++i) {
                children[i] = other.children[i];
            }
            prev = other.prev, next = other.next;
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
    bool insert_upper(node node1, info &v1, int &p1, info &v2, int &p2, info &vLast) { // 不用继续返回 false
        infoArr arr1;
        int pos = -1;
        information.read(arr1, node1.key);
        for (int i = 0; i < node1.size; ++i) {
            if (arr1.a[i] == vLast) {
                pos = i;
                break;
            }
        }
        vLast = arr1.a[0];
        for (int i = node1.size; i > pos + 1; --i) {
            arr1.a[i] = arr1.a[i - 1];
        }
        arr1.a[pos] = v1, arr1.a[pos + 1] = v2;
        for (int i = node1.size + 1; i > pos + 2; --i) {
            node1.children[i] = node1.children[i - 1];
        }
        node1.children[pos + 1] = p1, node1.children[pos + 2] = p2;
        if (node1.size < M) {
            node1.size++;
            information.write(arr1, node1.key), bpt.write(node1, node1.key);
            return false;
        } else {
            node1.size = (M + 1) / 2; 
            node node2;
            infoArr arr2;
            node2.size = M / 2, node2.key = ++tot;
            for (int i = node1.size + 1; i <= M; ++i) {
                arr2.a[i] = arr1.a[i - node1.size];
            }
            for (int i = node1.size + 1; i <= M + 1; ++i) {
                node2.children[i - node1.size - 1] = node1.children[i];
            }
            v1 = arr1.a[0], v2 = arr2.a[0];
            p1 = node1.key, p2 = tot;
            information.write(arr1, node1.key), bpt.write(node1, node1.key);
            information.write(arr2, tot), bpt.write(node2, tot);
            return true;
        }
    }
    void insert(T1 key, T2 val) {
        info info1 = info(key, val);
        if (!root) {
            node node1;
            infoArr arr1;
            node1.isLeaf = 1, node1.size = 1;
            node1.key = ++tot;
            arr1.a[0] = info1;
            information.write(arr1, tot);
            bpt.write(node1, tot);
            node rootNode;
            rootNode.size = 0, rootNode.key = ++tot;
            rootNode.children[0] = tot - 1;
            bpt.write(rootNode, tot);
        } else {
            infoArr arr1;
            node node1;
            bpt.read(node1, root);
            top = 0;
            while (!node1.isLeaf) {
                stack[top++] = node1;
                int pos = node1.size;
                information.read(arr1, node1.key);
                for (int i = 0; i < node1.size; ++i) {
                    if (info1 < arr1.a[i]) {
                        pos = i;
                        break;
                    }
                }
                bpt.read(node1, node1.children[pos]);
            }
            information.read(arr1, node1.key);
            int pos = node1.size;
            for (int i = 0; i < node1.size; ++i) {
                if (info1 < arr1.a[i]) {
                    pos = i;
                    break;
                }
            }
            info vLast = arr1.a[0];
            for (int i = M - 1; i > pos; --i) {
                arr1.a[i] = arr1.a[i - 1];
            }
            arr1.a[pos] = info1;
            if (node1.size < M) {
                node1.size++;
                information.write(arr1, node1.key);
                bpt.write(node1, node1.key);
            } else {
                node1.size = M / 2 + 1;
                node node2;
                infoArr arr2;
                node2.isLeaf = true, node2.size = (M + 1) / 2, node2.key = ++tot;
                node2.next = node1.next, node2.prev = node1.key;
                if (node1.next != -1) {
                    node nodeNext;
                    bpt.read(nodeNext, node1.next);
                    nodeNext.prev = tot;
                    bpt.write(nodeNext, node1.next);
                }
                node1.next = tot;
                for (int i = node1.size; i <= M; ++i) {
                    arr2.a[i - node1.size] = arr1.a[i];
                }
                information.write(arr1, node1.key), bpt.write(node1, node1.key);
                information.write(arr2, tot), bpt.write(node2, tot);
                info v1 = arr1.a[0], v2 = arr2.a[0];
                int p1 = node1.key, p2 = tot;
                for (int i = top - 1; i >= 0; ++i) {
                    if (!insert_upper(stack[i], v1, p1, v2, p2, vLast)) {
                        break;
                    }
                    if (i == 0) {
                        node rootNode;
                        infoArr rootArr;
                        rootNode.size = 1;
                        rootNode.children[0] = p1, rootNode.children[1] = p2;
                        rootNode.key = ++tot;
                        rootArr[0] = v2;
                        root = tot;
                        information.write(rootArr, tot), bpt.write(rootNode, tot);
                    }
                }
            }
        }
    }
    void myDelete(T1 key, T2 val) {

    }
    void show(T1 key) {
        info info1 = info(key, 0);
        infoArr arr1;
        node node1;
        bpt.read(node1, root);
        while (!node1.isLeaf) {
            information.read(arr1, node1.key);
            int pos = node1.size;
            for (int i = 0; i < node1.size; ++i) {
                if (info1 < arr1.a[i]) {
                    pos = i;
                    break;
                }
            }
            bpt.read(node1, node1.children[pos]);
        }
        bool flag = true, notNull = false;
        while (flag) {
            information.read(arr1, node1.key);
            for (int i = 0; i < node1.size; ++i) {
                if (arr1.a[i].key == key) {
                    cout << arr1.a[i].val << ' ';
                    notNull = true;
                } else if (key < arr1.a[i].key) {
                    flag = false;
                    break;
                }
            }
            if (node1.next == -1) {
                flag = false;
            } else {
                bpt.read(node1, node1.next);
            }
        }
        if (!notNull) {
            cout << "null";
        }
        cout << '\n';
    }
    void traverse() {

    }
};
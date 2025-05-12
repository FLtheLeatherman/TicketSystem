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
    bool insert_upper(node node1, bool &flag, info &v1, int &p1, info &v2, int &p2, info &vLast) { // 不用继续返回 false
        infoArr arr1;
        int pos = node1.size - 1;
        information.read(arr1, node1.key);
        for (int i = 0; i < node1.size; ++i) {
            if (vLast < arr.a[i]) {
                pos = i - 1;
                break;
            }
        }
        vLast = arr1.a[0];
        for (int i = node1.size; i > pos + 1; --i) {
            arr1.a[i] = arr1.a[i - 1];
        }
        if (flag && pos >= 0) arr1.a[pos] = v1;
        arr1.a[pos + 1] = v2;
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
                arr2.a[i - node1.size - 1] = arr1.a[i];
            }
            for (int i = node1.size + 1; i <= M + 1; ++i) {
                node2.children[i - node1.size - 1] = node1.children[i];
            }
            if (pos >= 0) {
                flag = false;
            }
            v2 = arr1.a[node1.size];
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
                bool flag = true;
                info v1 = arr1.a[0], v2 = arr2.a[0];
                int p1 = node1.key, p2 = tot;
                for (int i = top - 1; i >= 0; ++i) {
                    if (!insert_upper(stack[i], flag, v1, p1, v2, p2, vLast)) {
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
    // node1 底下一个为 vLast 的节点需要删除。
    bool deleteUpper(node node1, info &vLast) {
        infoArr arr1;
        information.read(arr1, node1.key);
        int pos = node1.size;
        for (int i = 0; i < node1.size; ++i) {
            if (arr1[i] > vLast) {
                pos = i;
                break;
            }
        }
        node node2, node3;
        infoArr arr2, arr3;
        if (pos == 0) {
            bpt.read(node2, node1.children[0]), bpt.read(node3, node1.children[1]);
            information.read(arr2, node2.key), information.read(arr3, node3.key);
            if (node3.size > (M + 1) / 2) {
                arr2.a[node2.size] = arr1.a[0];
                node2.children[node2.size + 1] = node3.children[0];
                arr1.a[0] = arr3.a[0];
                for (int i = 0; i < node3.size - 1; ++i) {
                    arr3.a[i] = arr3.a[i + 1];
                }
                for (int i = 0; i < node3.size; ++i) {
                    node3.children[i] = node3.children[i + 1];
                }
                node2.size++;
                node3.size--;
                information.write(arr1, node1.key), bpt.write(node1, node1.key);
                information.write(arr2, node2.key), bpt.write(node2, node2.key);
                information.write(arr3, node3.key), bpt.write(node3, node3.key);
                return false;
            } else {
                arr2.a[node2.size] = arr1.a[0];
                for (int i = node2.size + 1; i < node2.size + node3.size; ++i) {
                    arr2.a[i] = arr3.a[i - node2.size - 1];
                }
                for (int i = node2.size + 1; i <= node2 + node3.size + 1; ++i) {
                    node2.children[i] = node3.children[i - node2.size - 1];
                }
                for (int i = 0; i < node1.size - 1; ++i) {
                    arr1.a[i] = arr1.a[i + 1];
                }
                for (int i = 1; i < node1.size; ++i) {
                    node1.children[i] = node1.children[i + 1];
                }
                node1.size--;
                node2.size += node3.size + 1;
                information.write(arr1, node1.key), bpt.write(node1, node1.key);
                information.write(arr2, node2.key), bpt.write(node2, node2.key);
                if (node1.size < (M + 1) / 2) return true;
                else return false;
            }
        } else {
            bpt.read(node2, node1.children[pos]), bpt.read(node3, node1.children[pos - 1]);
            information.read(arr2, node2.key), information.read(arr3, node3.key);
            if (node3.size > (M + 1) / 2) {
                arr3.a[node3.size] = arr1.a[pos - 1];
                node3.children[node3.size + 1] = node2.children[0];
                node1.key[pos - 1] = node2.key[0];
                for (int i = 0; i < node2.size - 1; ++i) {
                    arr2.a[i] = arr2.a[i + 1];
                }
                for (int i = 0; i < node2.size; ++i) {
                    node2.children[i] = node2.children[i + 1];
                }
                node3.size++;
                node2.size--;
                information.write(arr1, node1.key), bpt.write(node1, node1.key);
                information.write(arr2, node2.key), bpt.write(node2, node2.key);
                information.write(arr3, node3.key), bpt.write(node3, node3.key);
                return false;
            } else {
                arr3.a[node3.size] = arr1.a[pos - 1];
                for (int i = node3.size + 1; i < node2.size + node3.size; ++i) {
                    arr3.a[i] = arr2.a[i - node3.size - 1];
                }
                for (int i = node3.size + 1; i <= node2 + node3.size + 1; ++i) {
                    node3.children[i] = node2.children[i - node3.size - 1];
                }
                for (int i = pos - 1; i < node1.size - 1; ++i) {
                    arr1.a[i] = arr1.a[i + 1];
                }
                for (int i = pos; i < node1.size; ++i) {
                    node1.children[i] = node1.children[i + 1];
                }
                node1.size--;
                node3.size += node2.size + 1;
                information.write(arr1, node1.key), bpt.write(node1, node1.key);
                information.write(arr3, node3.key), bpt.write(node3, node3.key);
                if (node1.size < (M + 1) / 2) return true;
                else return false;
            }
        }
    }
    void myDelete(T1 key, T2 val) {
        info info1 = info(key, val);
        node node1;
        infoArr arr1;
        if (!root) {
            return;
        } else {
            bpt.read(node1, root);
            bool isOne = node1.size == 0; // 根只有一个儿子了
            top = 0;
            while (!node1.isLeaf) {
                stack[top++] = node1;
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
            information.read(arr1, node1.key);
            int pos = node1.size;
            for (int i = 0; i < node1.size; ++i) {
                if (info1 == arr1.a[i]) {
                    pos = i;
                    break;
                }
            }
            if (pos == node1.size) return;
            if (pos == 0) {
                info vLast = arr1.a[0];
                infoArr tmp;
                information.read(tmp, stack[top - 1].key);
                for (int i = 0; i < stack[top - 1].size; ++i) {
                    if (tmp.a[i] == vLast) {
                        tmp.a[i] = arr1.a[1];
                    }
                }
                information.write(tmp, stack[top - 1].key);
            }
            for (int i = pos; i < node1.size - 1; ++i) {
                arr1.a[i] = arr1.a[i + 1];
            }
            information.write(arr1, node1.key);
            node1.size--;
            bpt.write(node1, node1.key);
            if (isOne) {
                if (node1.size == 0) {
                    root = 0;
                }
            } else if (node1.size < (M + 1) / 2) {
                info vLast = arr1.a[0];
                node node2 = stack[top - 1];
                infoArr arr2;
                information.read(arr2, node2.key);
                int pos = -1;
                for (int i = 0; i < node2.size; ++i) {
                    if (arr2.a[i] == vLast) {
                        pos = i;
                        break;
                    }
                }
                node node3;
                infoArr arr3;
                if (pos >= 0) {
                    bpt.read(node3, node2.children[pos]);
                    information.read(arr3, node3.key);
                    if (node3.size > (M + 1) / 2) {
                        for (int i = node1.size; i >= 1; --i) {
                            arr1.a[i] = arr1.a[i - 1];
                        }
                        arr1.a[0] = arr3.a[node3.size - 1];
                        node1.size++;
                        node3.size--;
                        arr2.a[pos] = arr1.a[0];
                        information.write(arr1, node1.key), bpt.write(node1, node1.key);
                        information.write(arr2, node2.key), bpt.write(node2, node2.key);
                        information.write(arr3, node3.key), bpt.write(node3, node3.key);
                    } else {
                        for (int i = node3.size; i < node3.size + node1.size; ++i) {
                            arr3.a[i] = arr1.a[i - node3.size];
                        }
                        node3.size += node1.size;
                        if (node1.next != -1) {
                            node node4;
                            bpt.read(node4, node1.next);
                            node4.prev = node3.key;
                            bpt.write(node4, node4.key);
                        }
                        node3.next = node1.next;
                        for (int i = pos; i < node2.size - 1; ++i) {
                            arr2.a[i] = arr2.a[i + 1];
                        }
                        for (int i = pos + 1; i < node2.size; ++i) {
                            node2.children[i] = node2.children[i + 1];
                        }
                        node2.size--;
                        information.write(arr2, node2.key), bpt.write(node2, node2.key);
                        information.write(arr3, node3.key), bpt.write(node3, node3.key);
                    }
                } else {
                    bpt.read(node3, node2.children[1]);
                    information.read(arr3, node3.key);
                    if (node3.size > (M + 1) / 2) {
                        arr1.a[node1.size] = arr3.a[0];
                        for (int i = 0; i < node3.size - 1; --i) {
                            arr3.a[i] = arr3.a[i + 1];
                        }
                        node1.size++;
                        node3.size--;
                        arr2.a[0] = arr3.a[0];
                        information.write(arr1, node1.key), bpt.write(node1, node1.key);
                        information.write(arr2, node2.key), bpt.write(node2, node2.key);
                        information.write(arr3, node3.key), bpt.write(node3, node3.key);
                    } else {
                        for (int i = node3.size; i < node3.size + node1.size; ++i) {
                            arr1.a[i] = arr3.a[i - node3.size];
                        }
                        node1.size += node3.size;
                        if (node3.next != -1) {
                            node node4;
                            bpt.read(node4, node3.next);
                            node4.prev = node1.key;
                            bpt.write(node4, node4.key);
                        }
                        node1.next = node3.next;
                        for (int i = 0; i < node2.size - 1; ++i) {
                            arr2.a[i] = arr2.a[i + 1];
                        }
                        for (int i = 1; i < node2.size; ++i) {
                            node2.children[i] = node2.children[i + 1];
                        }
                        node2.size--;
                        information.write(arr2, node2.key), bpt.write(node2, node2.key);
                        information.write(arr1, node1.key), bpt.write(node1, node1.key);
                    }
                }
                if (node2.size < (M + 1) / 2) {
                    vLast = arr2.a[0];
                    for (int i = top - 2; i >= 0; --i) {
                        if (!deleteUpper(stack[i], vLast)) {
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
};
#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>

namespace sjtu {
/**
 * @brief a container like std::priority_queue which is a heap internal.
 * **Exception Safety**: The `Compare` operation might throw exceptions for certain data.
 * In such cases, any ongoing operation should be terminated, and the priority queue should be restored to its original state before the operation began.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
    /**
     * @brief default constructor
     */
    priority_queue() : root(nullptr), siz(0) {}

    /**
     * @brief copy constructor
     * @param other the priority_queue to be copied
     */
    priority_queue(const priority_queue &other) : root(copy(other.root)), siz(other.siz) {}

    /**
     * @brief deconstructor
     */
    ~priority_queue() {
        clear(root);
        siz = 0;
    }

    /**
     * @brief Assignment operator
     * @param other the priority_queue to be assigned from
     * @return a reference to this priority_queue after assignment
     */
    priority_queue &operator=(const priority_queue &other) {
        if (this == &other) {
            return *this;
        }
        clear(root);
        root = copy(other.root);
        siz = other.siz;
        return *this;
    }

    /**
     * @brief get the top element of the priority queue.
     * @return a reference of the top element.
     * @throws container_is_empty if empty() returns true
     */
    const T & top() const {
        // if (siz == 0) {
        //     throw container_is_empty();
        // }
        // try {
            return root->data;
        // } catch (...) {
        //     throw;
        // }
    }

    /**
     * @brief push new element to the priority queue.
     * @param e the element to be pushed
     */
    void push(const T &e) {
        // std::cout << "so far so good" << std::endl;
        node *tmp = new node(e);
        siz++;
        // try {
            root = merge(root, tmp);
        // } catch (...) {
        //     delete tmp;
        //     siz--;
        //     throw;
        // }
    }

    /**
     * @brief delete the top element from the priority queue.
     * @throws container_is_empty if empty() returns true
     */
    void pop() {
        // if (siz == 0) {
        //     throw container_is_empty();
        // }
        // try {
            node *tmp = root;
            // std::cout << (root->lson == nullptr) << std::endl;
            // std::cout << (root->rson == nullptr) << std::endl;
            root = merge(root->lson, root->rson);
            // std::cout << (root == nullptr) << std::endl;
            delete tmp;
            siz--;
        // } catch (...) {
        //     throw;
        // }
    }

    /**
     * @brief return the number of elements in the priority queue.
     * @return the number of elements.
     */
    size_t size() const {
        return siz;
    }

    /**
     * @brief check if the container is empty.
     * @return true if it is empty, false otherwise.
     */
    bool empty() const {
        return siz == 0;
    }

    /**
     * @brief merge another priority_queue into this one.
     * The other priority_queue will be cleared after merging.
     * The complexity is at most O(logn).
     * @param other the priority_queue to be merged.
     */
    void merge(priority_queue &other) {
        if (this == &other) {
            return;
        }
        // try {
            root = merge(root, other.root);
            siz += other.siz;
            other.root = nullptr;
            other.siz = 0;
        // } catch (...) {
        //     throw;
        // }
    }
private:
    size_t siz;
    struct node {
        T data;
        node *lson, *rson;
        node(const T &data) : data(data), lson(nullptr), rson(nullptr) {}
    };
    node *root;
    node *copy(node *now) {
        node *tmp = new node(now->data);
        if (now->lson != nullptr) {
            tmp->lson = copy(now->lson);
        }
        if (now->rson != nullptr) {
            tmp->rson = copy(now->rson);
        }
        return tmp;
    }
    void clear(node *now) {
        if (now == nullptr) {
            return;
        }
        if (now->lson != nullptr) {
            clear(now->lson);
            now->lson = nullptr;
        }
        if (now->rson != nullptr) {
            clear(now->rson);
            now->rson = nullptr;
        }
        delete now;
        now = nullptr;
    }
    node *merge(node *now1, node *now2) {
        if (now1 == nullptr) {
            return now2;
        }
        if (now2 == nullptr) {
            return now1;
        }
        // try {
            if (Compare()(now1->data, now2->data)) {
                std::swap(now1, now2);
            }
            node *tmp = merge(now1->rson, now2);
            now1->rson = tmp;
            std::swap(now1->lson, now1->rson);
            return now1;
        // } catch (...) {
        //     throw;
        }
};

}

#endif
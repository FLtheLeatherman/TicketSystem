#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "exceptions.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
        public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;
        class iterator {
        // The following code is written for the C++ type_traits library.
        // Type traits is a C++ feature for describing certain properties of a type.
        // For instance, for an iterator, iterator::value_type is the type that the
        // iterator points to.
        // STL algorithms and containers may use these type_traits (e.g. the following
        // typedef) to work properly. In particular, without the following code,
        // @code{std::sort(iter, iter1);} would not compile.
        // See these websites for more information:
        // https://en.cppreference.com/w/cpp/header/type_traits
        // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
        // About iterator_category: https://en.cppreference.com/w/cpp/iterator
            public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::random_access_iterator_tag;

            private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            pointer p;

            public:
            iterator() : p(nullptr) {}
            iterator(pointer p) : p(p) {}
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */

            iterator operator+(const int &n) const {
                return iterator(p + n);
            }

            iterator operator-(const int &n) const {
                return iterator(p - n);
            }

            int operator-(const iterator &rhs) const {
                return p - rhs.p;
            }

            iterator& operator+=(const int &n) {
                p += n;
                return *this;
            }

            iterator& operator-=(const int &n) {
                p -= n;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp(*this);
                ++p;
                return tmp;
            }

            iterator& operator++() {
                ++p;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp(*this);
                --p;
                return tmp;
            }

            iterator& operator--() {
                --p;
                return *this;
            }

            T& operator*() const {
                return *p;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return p == rhs.p;
            }

            bool operator==(const const_iterator &rhs) const {
                return p == rhs.p;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return p != rhs.p;
            }

            bool operator!=(const const_iterator &rhs) const {
                return p != rhs.p;
            }
        };

        class const_iterator {
            public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;
            using iterator_category = std::random_access_iterator_tag;

            private:
            pointer p;

            public:
            const_iterator() : p(nullptr) {}
            const_iterator(pointer p) : p(p) {}
            const_iterator(const iterator &it) : p(it.p) {}

            const_iterator operator+(const int &n) const {
                return const_iterator(p + n);
            }

            const_iterator operator-(const int &n) const {
                return const_iterator(p - n);
            }

            int operator-(const const_iterator &rhs) const {
                return p - rhs.p;
            }

            const_iterator& operator+=(const int &n) {
                p += n;
                return *this;
            }

            const_iterator& operator-=(const int &n) {
                p -= n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp(*this);
                ++p;
                return tmp;
            }

            const_iterator& operator++() {
                ++p;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp(*this);
                --p;
                return tmp;
            }

            const_iterator& operator--() {
                --p;
                return *this;
            }

            const T& operator*() const {
                return *p;
            }

            bool operator==(const iterator &rhs) const {
                return p == rhs.p;
            }

            bool operator==(const const_iterator &rhs) const {
                return p == rhs.p;
            }

            bool operator!=(const iterator &rhs) const {
                return p != rhs.p;
            }

            bool operator!=(const const_iterator &rhs) const {
                return p != rhs.p;
            }
        };

        private:
		/*TODO*/
        T* data;
        size_t capacity;
        size_t current_size;

        // Resize the array to a new capacity
        void resize(size_t new_capacity) {
            // Allocate raw memory without calling the constructor
            void* raw_data = std::malloc(new_capacity * sizeof(T));
            T* new_data = static_cast<T*>(raw_data);
            
            // Manually construct objects in the allocated memory
            for (size_t i = 0; i < current_size; i++) {
                new(&new_data[i]) T(std::move(data[i]));  // Placement new to construct objects
                data[i].~T();  // Destroy the old object
            }

            // Deallocate old memory
            std::free(data);

            data = new_data;
            capacity = new_capacity;
        }
        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        public:
        vector() : data(nullptr), capacity(0), current_size(0) {}

        vector(const vector &other) : data(nullptr), capacity(0), current_size(0) {
            *this = other;
        }
        ~vector() {
            clear();
            std::free(data);
        }
        vector& operator=(const vector &other) {
            if (this == &other) return *this;
            clear();
            capacity = other.capacity;
            current_size = other.current_size;
            data = static_cast<T*>(std::malloc(capacity * sizeof(T)));
            for (size_t i = 0; i < current_size; i++) {
                new(&data[i]) T(other.data[i]);  // Use placement new
            }
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */

        T& at(const size_t &pos) {
            if (pos >= current_size) throw index_out_of_bound();
            return data[pos];
        }

        const T& at(const size_t &pos) const {
            if (pos >= current_size) throw index_out_of_bound();
            return data[pos];
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */

        T& operator[](const size_t &pos) {
            return data[pos];
        }

        const T& operator[](const size_t &pos) const {
            return data[pos];
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T& front() const {
            if (empty()) throw container_is_empty();
            return data[0];
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T& back() const {
            if (empty()) throw container_is_empty();
            return data[current_size - 1];
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(data);
        }

        const_iterator begin() const {
            return const_iterator(data);
        }

        const_iterator cbegin() const {
            return const_iterator(data);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(data + current_size);
        }

        const_iterator end() const {
            return const_iterator(data + current_size);
        }

        const_iterator cend() const {
            return const_iterator(data + current_size);
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return current_size == 0;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return current_size;
        }
        /**
         * clears the contents
         */
        void clear() {
            for (size_t i = 0; i < current_size; i++) {
                data[i].~T();  // Explicit destructor call
            }
            current_size = 0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            size_t index = pos - begin();
            if (current_size == capacity) resize(capacity == 0 ? 1 : capacity * 2);
            for (size_t i = current_size; i > index; i--) {
                new(&data[i]) T(std::move(data[i - 1]));
                data[i - 1].~T();
            }
            new(&data[index]) T(value);
            current_size++;
            return iterator(data + index);
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > current_size) throw index_out_of_bound();
            return insert(begin() + ind, value);
        }
        /**
        * removes the element at pos.
        * return an iterator pointing to the following element.
        * If the iterator pos refers the last element, the end() iterator is returned.
        */
        iterator erase(iterator pos) {
            size_t index = pos - begin();
            data[index].~T();
            for (size_t i = index; i < current_size - 1; i++) {
                new(&data[i]) T(std::move(data[i + 1]));
                data[i + 1].~T();
            }
            current_size--;
            return iterator(data + index);
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind >= current_size) throw index_out_of_bound();
            return erase(begin() + ind);
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (current_size == capacity) resize(capacity == 0 ? 1 : capacity * 2);
            new(&data[current_size]) T(value);
            current_size++;
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (empty()) throw container_is_empty();
            data[current_size - 1].~T();
            current_size--;
        }

        void swap(vector &rhs) {
            T *tmpptr = data;
            data = rhs.data;
            rhs.data = tmpptr;
            size_t tmp = capacity;
            capacity = rhs.capacity;
            rhs.capacity = tmp;
            tmp = current_size;
            current_size = rhs.current_size;
            rhs.current_size = tmp;
        }

        bool operator == (const vector &rhs) const {
            if (this == &rhs) return true;
            if (size() != rhs.size()) return false;
            for (auto it1 = cbegin(), it2 = rhs.cbegin(); it1 != cend(); ++it1, ++it2)
                if (*it1 != *it2) return false;
            return true;
        }
        bool operator < (const vector &rhs) const {
            for (auto it1 = cbegin(), it2 = rhs.cbegin(); it1 != cend() && it2 != rhs.cend(); ++it1, ++it2)
                if (*it1 != *it2) return *it1 < *it2;
            if (size() != rhs.size()) return size() < rhs.size();
            return false;
        }
        bool operator != (const vector &rhs) const {
            return !(*this == rhs);
        }
        bool operator > (const vector &rhs) const {
            return rhs < *this;
        }
        bool operator <= (const vector &rhs) const {
            return !(rhs < *this);
        }
        bool operator >= (const vector &rhs) const {
            return !(*this < rhs);
        }
    };

}

#endif
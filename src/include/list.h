/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-05 22:12:06
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__
#include "io.h"
template <typename T> struct __list_node {
    T val;

    __list_node<T> *next = nullptr;
    __list_node<T> *prev = nullptr;
};

template <typename T> struct __list_iterator {
    __list_node<T> *node;

    __list_iterator(__list_node<T> *_node) : node(_node) {}

    // iter++
    __list_iterator<T> operator++(int) {
        __list_iterator<T> __tmp = *this;
        node = node->next;
        return __tmp;
    }

    //++iter
    __list_iterator<T> &operator++() {
        node = node->next;
        return *this;
    }

    // iter--
    __list_iterator<T> operator--(int) {
        __list_iterator<T> __tmp = *this;
        node = node->prev;
        return __tmp;
    }

    //--iter
    __list_iterator<T> &operator--() {
        node = node->prev;
        return *this;
    }

    T &operator*() const { return node->val; }
    bool operator==(const __list_iterator &x) { return this->node == x.node; }
    bool operator!=(const __list_iterator &x) { return this->node != x.node; }
};

template <typename T> class list {
  public:
    __list_node<T> node;
    unsigned int __size = 0;

  public:
    typedef __list_iterator<T> iterator;

    list() {
        printk_svga("constructor");
        node.next = &node;
        node.prev = &node;
    }

    // void push_back(T _val) {}
    // void push_front(T _val) {}

    T &front() { return *begin(); }
    T &back() { return *(--end()); }

    unsigned int size() const { return __size; }
    bool empty() const { return node.next == &node; }

    // bool remove(T _tar) { // TODO free memory
    //     __list_node<T> *tmp = head;
    //     while (tmp != nullptr) {
    //         if (tmp->val == _tar) {
    //             if (tmp == head)
    //                 head = tmp->next;
    //             else if (tmp == tail)
    //                 tmp->prev->next = nullptr;
    //             else {
    //                 tmp->prev->next = tmp->next;
    //                 tmp->next->prev = tmp->prev;
    //             }
    //             return true;
    //         }
    //     }

    //     return false;
    // }

    iterator begin() { return iterator(node.next); }
    iterator end() { return iterator(&node); }

    void __push_back_(__list_node<T> *_node) {
        printk_svga("this obj addr:0x%x\n", this);

        node.prev->next = _node;
        _node->prev = node.prev;
        node.prev = _node;
        _node->next = &node;

        __size++;
    }
};
#endif

/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-12 17:37:58
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

#include "stdint.h"

template <typename T> struct __list_node {
    T val;
    __list_node<T> *prev;
    __list_node<T> *next;

    __list_node() { prev = next = this; }
};

template <typename T> struct __list_iterator {
    __list_node<T> *node;

    __list_iterator(__list_node<T> *_node) : node(_node) {}

    // iter++
    __list_iterator<T> operator++(int) {
        __list_iterator<T> temp = *this;
        node = node->next;
        return temp;
    }

    //++iter
    __list_iterator<T> operator++() {
        node = node->next;
        return *this;
    }

    // iter--
    __list_iterator<T> operator--(int) {
        __list_iterator<T> temp = *this;
        node = node->next;
        return temp;
    }

    //--iter
    __list_iterator<T> operator--() {
        node = node->next;
        return *this;
    }

    T &operator*() const { return node->val; }
    bool operator==(const __list_iterator &x) { return this->node == x.node; }
    bool operator!=(const __list_iterator &x) { return this->node != x.node; }
};

template <typename T> class list {
  private:
    __list_node<T> _head;
    __list_node<T> *_list;

    uint32_t _size;

  public:
    typedef __list_iterator<T> iterator;

    iterator begin() { return iterator(_list->next); }
    iterator end() { return iterator(_list); }

    uint64_t size() { return _size; }
    bool empty() { return _list->next == _list; }

    void push_back(T _val) {}

    // add a list_node to the tail of the list
    void __list_add(__list_node<T> *);

    list();
};

template <typename T> list<T>::list() { _list = &_head; }

template <typename T> void list<T>::__list_add(__list_node<T> *_node) {
    _list->prev->next = _node;
    _node->prev = _list->prev;
    _list->prev = _node;
    _node->next = _list;

    _size++;
}

#endif
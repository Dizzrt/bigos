/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-13 22:15:34
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__
#include "MMU/memory.h"
#include "io.h"
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

    typedef __list_node<T> *link_type;
    link_type creat_node(const T &x) {
        link_type node = (link_type)kmalloc(sizeof(__list_node<T>));
        node->next = node->prev = node;
        node->val = x;

        return node;
    }

  public:
    typedef __list_iterator<T> iterator;

    iterator begin() { return iterator(_list->next); }
    iterator end() { return iterator(_list); }

    uint64_t size() { return _size; }
    bool empty() { return _list->next == _list; }

    void insert(const iterator &, const T &);
    void push_front(const T &x) { insert(begin(), x); }
    void push_back(const T &x) { insert(end(), x); }

    // add a list_node to the list
    void __list_add(link_type, const iterator &);

    list();
};

template <typename T> list<T>::list() { _list = &_head; }

template <typename T> void list<T>::__list_add(link_type _node, const iterator &position) {
    position.node->prev->next = _node;
    _node->prev = position.node->prev;
    position.node->prev = _node;
    _node->next = position.node;

    _size++;
}

template <typename T> void list<T>::insert(const iterator &position, const T &x) {
    link_type tmp = creat_node(x);
    __list_add(tmp, position);
    return;
}

#endif
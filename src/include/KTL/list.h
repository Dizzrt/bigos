/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-14 17:54:19
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__
#include "MMU\memory.h"
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
        node = node->prev;
        return temp;
    }

    //--iter
    __list_iterator<T> operator--() {
        node = node->prev;
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

    uint32_t size() { return _size; }
    bool empty() { return _list->next == _list; }

    void insert(const iterator &, const T &);
    void push_front(const T &x) { insert(begin(), x); }
    void push_back(const T &x) { insert(end(), x); }
    void erase(const iterator &);
    void erase(const iterator &, const iterator &);
    void reverse();
    void pop_back() { erase(--end()); }
    void pop_front() { erase(begin()); }

    // add a list_node to the list
    void __list_add(link_type, const iterator &);
    void __list_rm(const iterator &);

    list();
    // destructor need '__cxa_atexit'
    //~list() {}
};

template <typename T> list<T>::list() { _list = &_head; }

template <typename T> void list<T>::__list_add(link_type _node, const iterator &position) {
    position.node->prev->next = _node;
    _node->prev = position.node->prev;
    position.node->prev = _node;
    _node->next = position.node;

    _size++;
}

template <typename T> void list<T>::__list_rm(const iterator &x) {
    x.node->prev->next = x.node->next;
    x.node->next->prev = x.node->prev;

    _size--;
}

template <typename T> void list<T>::insert(const iterator &position, const T &x) {
    link_type tmp = creat_node(x);
    __list_add(tmp, position);
    return;
}

template <typename T> void list<T>::erase(const iterator &x) {
    __list_rm(x);

    // TODO kfree
}

template <typename T> void list<T>::erase(const iterator &x, const iterator &y) {
    uint32_t _t = 1;
    for (list<T>::iterator i = x; i != y; i++)
        _t++;
    _size -= _t;

    x.node->prev->next = y.node->next;
    y.node->next->prev = x.node->prev;

    // TODO kfree
}

template <typename T> void list<T>::reverse() {
    link_type cur = _list;
    link_type next = cur->next;

    do {
        cur->next = cur->prev;
        cur->prev = next;

        cur = next;
        next = cur->next;

    } while (cur != _list);
    return;
}

#endif
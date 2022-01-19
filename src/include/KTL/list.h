#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

#include "KTL\linked_container.h"
#include "MMU\memory.h"
#include "stdint.h"

template <typename T>
class list {
  private:
    linked_container<T> _head;
    linked_container<T>* _list;

    uint32_t _size;

    typedef linked_container<T>* link_type;
    link_type creat_node(const T& x) {
        link_type m_node = (link_type)kmalloc(sizeof(linked_container<T>));
        m_node->next = m_node->prev = m_node;
        m_node->val = x;

        return m_node;
    }

  public:
    typedef linked_container_iterator<T> iterator;

    iterator begin() { return iterator(_list->next); }
    iterator end() { return iterator(_list); }

    uint32_t size() { return _size; }
    bool empty() { return _list->next == _list; }

    void insert(const iterator&, const T&);
    void push_front(const T& x) { insert(begin(), x); }
    void push_back(const T& x) { insert(end(), x); }
    void erase(const iterator&);
    void erase(const iterator&, const iterator&);
    void reverse();
    void pop_back() { erase(--end()); }
    void pop_front() { erase(begin()); }

    T& front() { return *begin(); }
    T& back() { return *(--end()) }

    // add a list_node to the list
    void __list_add(link_type, const iterator&);
    void __list_rm(const iterator&);

    list();
    // destructor need '__cxa_atexit'
    //~list() {}
};

template <typename T>
list<T>::list() {
    _list = &_head;
}

template <typename T>
void list<T>::__list_add(link_type _node, const iterator& position) {
    position.m_node->prev->next = _node;
    _node->prev = position.m_node->prev;
    position.m_node->prev = _node;
    _node->next = position.m_node;

    _size++;
}

template <typename T>
void list<T>::__list_rm(const iterator& x) {
    x.m_node->prev->next = x.m_node->next;
    x.m_node->next->prev = x.m_node->prev;

    _size--;
}

template <typename T>
void list<T>::insert(const iterator& position, const T& x) {
    link_type tmp = creat_node(x);
    __list_add(tmp, position);
    return;
}

template <typename T>
void list<T>::erase(const iterator& x) {
    __list_rm(x);

    // TODO kfree
}

template <typename T>
void list<T>::erase(const iterator& x, const iterator& y) {
    uint32_t _t = 1;
    for (list<T>::iterator i = x; i != y; i++)
        _t++;
    _size -= _t;

    x.m_node->prev->next = y.m_node->next;
    y.m_node->next->prev = x.m_node->prev;

    // TODO kfree
}

template <typename T>
void list<T>::reverse() {
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
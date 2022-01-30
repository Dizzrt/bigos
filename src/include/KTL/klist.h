#ifndef __BIG_KLIST_H__
#define __BIG_KLIST_H__

#include "linked_container.h"
#include "stdint.h"

template <typename T>
class klist {
  protected:
    linked_container<T> _head;
    linked_container<T>* _list;

    uint32_t _size;
    typedef linked_container<T>* link_type;

  public:
    typedef linked_container_iterator<T> iterator;

    iterator begin() { return iterator(_list->next); }
    iterator end() { return iterator(_list); }

    uint32_t size() { return _size; }
    bool empty() { return _list->next == _list; }

    void reverse();
    T& front() { return *begin(); }
    T& back() { return *(--end()); }

    void __list_insert(link_type, const iterator&);
    T& __list_remove(const iterator&);

    klist();
    ~klist() = default;
};

template <typename T>
klist<T>::klist() {
    _list = &_head;
}

template <typename T>
void klist<T>::__list_insert(link_type _node, const iterator& position) {
    position.m_node->prev->next = _node;
    _node->prev = position.m_node->prev;
    position.m_node->prev = _node;
    _node->next = position.m_node;

    _size++;
}

template <typename T>
T& klist<T>::__list_remove(const iterator& x) {
    x.m_node->prev->next = x.m_node->next;
    x.m_node->next->prev = x.m_node->prev;

    _size--;
    return *x;
}

template <typename T>
void klist<T>::reverse() {
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
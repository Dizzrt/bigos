#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

#include "MMU\memory.h"
#include "klist.h"

template <typename T>
class list : public klist<T> {
  private:
    typedef linked_container<T>* link_type;
    link_type creat_node(const T& x) {
        link_type m_node = (link_type)kmalloc(sizeof(linked_container<T>));
        m_node->next = m_node->prev = m_node;
        m_node->val = x;

        return m_node;
    }

  public:
    list(/* args */){};
    ~list() = default;

    typedef linked_container_iterator<T> iterator;

    void insert(const iterator&, const T&);
    void erase(const iterator&);
    void erase(const iterator&, const iterator&);

    void push_back(const T& x) { insert(end(), x); }
    void push_front(const T& x) { insert(begin(), x); }
    void pop_back() { erase(--end()); }
    void pop_front() { erase(begin()); }
};

template <typename T>
void list<T>::insert(const iterator& position, const T& x) {
    link_type tmp = creat_node(x);
    __list_insert(tmp, position);
    return;
}

template <typename T>
void list<T>::erase(const iterator& x) {
    __list_remove(x);

    // TODO kfree
}

template <typename T>
void list<T>::erase(const iterator& x, const iterator& y) {
    uint32_t _t = 1;
    for (klist<T>::iterator i = x; i != y; i++)
        _t++;
    _size -= _t;

    x.m_node->prev->next = y.m_node->next;
    y.m_node->next->prev = x.m_node->prev;

    // TODO kfree
}

#endif
/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-11 20:22:38
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

#define offsetof(TYPE, MEMBER) ((long long)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)                                                                                \
    ({                                                                                                                 \
        const typeof(((type *)0)->member) *__mptr = (ptr);                                                             \
        (type *)((char *)__mptr - offsetof(type, member));                                                             \
    })

struct list_node {
    list_node *prev;
    list_node *next;

    list_node() { prev = next = this; }
};

template <typename T> struct list_iterator {
    list_node *node;

    list_iterator(list_node *_node) : node(_node) {}

    // iter++
    list_iterator<T> operator++(int) {
        list_iterator<T> temp = *this;
        node = node->next;
        return temp;
    }

    //++iter
    list_iterator<T> &operator++() {
        node = node->next;
        return *this;
    }

    // iter--
    list_iterator<T> operator--(int) {
        list_iterator<T> temp = *this;
        node = node->prev;
        return temp;
    }

    //--iter
    list_iterator<T> &operator--() {
        node = node->prev;
        return *this;
    }

    T *operator*() const { return (T *)container_of(node, T, list); }
    bool operator==(const list_iterator &x) { return this->node == x.node; }
    bool operator!=(const list_iterator &x) { return this->node != x.node; }
};

void list_add(list_node *, list_node *);

void list_remove(list_node *);

#endif

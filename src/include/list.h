/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-02 13:22:04
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

// #define LIST_HEAD_INIT(name)                                                                                           \
//     { &(name), &(name) }
// #define LIST_HEAD(name) list_head name = LIST_HEAD_INIT(name)

// struct list_head {
//     list_head *next, *prev;
// };

// static inline void __list_add(list_head *node, list_head *prev, list_head *next);

// static inline void list_add(list_head *node, list_head *head);

template <typename T> struct __list_node {
    __list_node<T> *prev;
    __list_node<T> *next;

    T val;
};

template <typename T> class list {
  private:
    __list_node<T> *__list;

  public:
    void push_back(const T &);
};

template <typename T> void list<T>::push_back(const T &val) { __list->val = val; }

#endif

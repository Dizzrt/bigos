/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-04 22:17:14
 */

#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

struct __list_node_base {
    __list_node_base *next = nullptr;
    __list_node_base *prev = nullptr;
};

template <typename T> struct __list_node : public __list_node_base { T val; };

template <typename T> struct __list_iterator {
    __list_node_base *node;

    __list_iterator(__list_node_base *_node) { this->node = _node; }

    T &operator*() const { return static_cast<__list_node<T> *>(node)->val; }

    // iter++
    __list_iterator<T> operator++(int) {
        __List_iterator<T> __tmp = *this;
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
};

template <typename T> class list {
  private:
    __list_node<T> *head = nullptr;
    __list_node<T> *tail = nullptr;

    unsigned int __size = 0;

  public:
    typedef __list_iterator<T> iterator;

    list() {}

    void push_back(T _val) {}
    void push_front(T _val) {}

    unsigned int size() { return __size; }
    bool empty() { return __size == 0 ? true : false; }

    bool remove(T _tar) { // TODO free memory
        __list_node<T> *tmp = head;
        while (tmp != nullptr) {
            if (tmp->val == _tar) {
                if (tmp == head)
                    head = tmp->next;
                else if (tmp == tail)
                    tmp->prev->next = nullptr;
                else {
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                }
                return true;
            }
        }
        return false;
    }

    iterator begin() { return iterator(this->head); }

    void __list_init(__list_node<T> *_val) {
        head = tail = _val;
        __size = 1;
    }
};
#endif

#ifndef __BIG_LINKED_CONTAINER_H__
#define __BIG_LINKED_CONTAINER_H__

template <typename T>
struct linked_container {
    T val;

    linked_container<T>* prev;
    linked_container<T>* next;

    linked_container(const T& _val) : val(_val) { prev = next = this; }
    linked_container() : linked_container(T()) {}
};

template <typename T>
struct linked_container_iterator {
    linked_container<T>* m_node;

    linked_container_iterator() = default;
    linked_container_iterator(linked_container<T>* _node) : m_node(_node) {}

    // iter++
    linked_container_iterator<T> operator++(int) {
        linked_container_iterator<T> temp = *this;
        m_node = m_node->next;
        return temp;
    }

    //++iter
    linked_container_iterator<T> operator++() {
        m_node = m_node->next;
        return *this;
    }

    // iter--
    linked_container_iterator<T> operator--(int) {
        linked_container_iterator<T> temp = *this;
        m_node = m_node->prev;
        return temp;
    }

    //--iter
    linked_container_iterator<T> operator--() {
        m_node = m_node->prev;
        return *this;
    }

    T& operator*() const { return m_node->val; }
    bool operator==(const linked_container_iterator& x) { return this->m_node == x.m_node; }
    bool operator!=(const linked_container_iterator& x) { return this->m_node != x.m_node; }
};

#endif
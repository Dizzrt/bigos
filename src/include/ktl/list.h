#ifndef __BIG_LIST_H__
#define __BIG_LIST_H__

#include "klist.h"
#include "ktl_allocator.h"

namespace kstd
{

    template<typename _Tp, template<typename> class _Alloc = kstd::allocator>
    class list {
    public:
        typedef _Tp* pointer;
        typedef const _Tp* const_pointer;
        typedef _Tp& reference;
        typedef const _Tp& const_reference;

        typedef _Tp value_type;
        typedef linked_container<_Tp> node_type;
        typedef linked_container_iterator<_Tp> iterator;
    private:
        klist<_Tp> _list;
        kstd::Allocator<node_type, _Alloc> allocator;
    public:
        // iterator
        iterator begin();
        iterator end();

        bool empty();
        size_t size();

        reference front() noexcept { return *begin(); }
        const_reference front() const noexcept { return *begin(); }

        reference back() noexcept { return *(--end()); }
        const_reference back() const noexcept { return *(--end()); }

        // push & insert
        void push_front(const value_type& __x);
        void push_front(value_type&& __x);//MARKER std::move

        template<typename... _Args>
        reference emplace_front(_Args&&... __args);//MARKER std::forward

        void push_back(const value_type& __x);
        void push_back(value_type&& __x);

        template<typename... _Args>
        reference emplace_back(_Args&&... __args);



        // pop & erase
        void pop_front() noexcept {}
        void pop_back() noexcept {}
    };

} // namespace kstd




// template <typename T>
// class list : public klist<T> {
// private:
//     typedef linked_container<T>* link_type;
//     link_type creat_node(const T& x) {
//         link_type m_node = (link_type)kmalloc(sizeof(linked_container<T>));
//         m_node->next = m_node->prev = m_node;
//         m_node->val = x;

//         return m_node;
//     }

// public:
//     list(/* args */) {};
//     ~list() = default;

//     typedef linked_container_iterator<T> iterator;

//     void insert(const iterator&, const T&);
//     void erase(const iterator&);
//     void erase(const iterator&, const iterator&);

//     void push_back(const T& x) { insert(end(), x); }
//     void push_front(const T& x) { insert(begin(), x); }
//     void pop_back() { erase(--end()); }
//     void pop_front() { erase(begin()); }
// };

// template <typename T>
// void list<T>::insert(const iterator& position, const T& x) {
//     link_type tmp = creat_node(x);
//     __list_insert(tmp, position);
//     return;
// }

// template <typename T>
// void list<T>::erase(const iterator& x) {
//     __list_remove(x);

//     // TODO kfree
// }

// template <typename T>
// void list<T>::erase(const iterator& x, const iterator& y) {
//     uint32_t _t = 1;
//     for (klist<T>::iterator i = x; i != y; i++)
//         _t++;
//     _size -= _t;

//     x.m_node->prev->next = y.m_node->next;
//     y.m_node->next->prev = x.m_node->prev;

//     // TODO kfree
// }

#endif
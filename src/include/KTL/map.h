#ifndef __BIG_MAP_H__
#define __BIG_MAP_H__
#include "MMU\memory.h"
#include "io.h"
#include "pair.h"

template <typename TKEY, typename TVAL>
struct __map_node {
    bool isRed = true;
    pair<const TKEY, TVAL> val;

    __map_node* left;
    __map_node* right;
    __map_node* father;
};

template <typename TKEY, typename TVAL>
struct __map_iterator {
    typedef __map_node<TKEY, TVAL> node_type;

    node_type* nil;
    node_type* m_node;

    __map_iterator(node_type* _node) : m_node(_node) {
        // end()
        if ((_node->father)->father == _node)
            nil = _node->father;
        // begin()
        else
            nil = _node->left;
    }

    void __increment() {
        if (m_node->right != nil) {
            m_node = m_node->right;
            while (m_node->left != nil)
                m_node = m_node->left;
        } else {
            node_type* f = m_node->father;
            printk_svga("0x%llx--0x%llx\n", m_node, f);
            while (m_node == f->right) {
                m_node = f;
                f = f->father;

                printk_svga("0x%llx--0x%llx\n", m_node, f);
            }
            if (m_node->right != f)
                m_node = f;
        }
    }

    void __decrement() {
        if (m_node->isRed && m_node->father->father == m_node)
            m_node = m_node->right;
        else if (m_node->left != nil) {
            node_type* temp = m_node->left;
            while (temp->right != nil)
                temp = temp->right;
            m_node = temp;
        } else {
            node_type* temp = m_node->father;
            while (m_node == temp->left) {
                m_node = temp;
                temp = temp->father;
            }
            m_node = temp;
        }
    }

    __map_iterator<TKEY, TVAL> operator++(int) {
        __map_iterator<TKEY, TVAL> temp = *this;
        __increment();
        return temp;
    }

    __map_iterator<TKEY, TVAL> operator++() {
        __increment();
        return *this;
    }

    __map_iterator<TKEY, TVAL> operator--(int) {
        __map_iterator<TKEY, TVAL> temp = *this;
        __decrement();
        return temp;
    }

    __map_iterator<TKEY, TVAL> operator--() {
        __increment();
        return *this;
    }

    pair<const TKEY, TVAL>& operator*() const { return m_node->val; }
    bool operator==(const __map_iterator& x) { return this->m_node == x.m_node; }
    bool operator!=(const __map_iterator& x) { return this->m_node != x.m_node; }
};

template <typename TKEY, typename TVAL>
class map {
  private:
    typedef __map_node<TKEY, TVAL> node_type;

    node_type* root;

    node_type nilNode;
    // root's left points to the most left node whitch is the first node in LDR
    // root's right points to the most right node whitch is the last node in LDR
    // root's father points to the top node
    unsigned int _size;

    void __insert(node_type*);
    void __remove(node_type*);

    node_type* __search(TKEY, bool = false);
    node_type* __leftmost();
    node_type* __rightmost();

    void __left_rotate(node_type*);
    void __right_rotate(node_type*);

    void __fixup(node_type*);

  public:
    node_type* nil;
    typedef __map_iterator<TKEY, TVAL> iterator;

    map();
    //~map();

    TVAL& operator[](const TKEY& key) {
        node_type* ret = __search(key);
        if (ret == nil) {
            ret = (node_type*)kmalloc(sizeof(node_type));
            const_cast<TKEY&>(ret->val.first) = key;
            ret->val.second = TVAL();
            ret->isRed = true;
            ret->left = ret->right = nil;

            __insert(ret);
        }

        return ret->val.second;
    }

    bool count(const TKEY& key) {}
    TVAL& find(const TKEY& key) { return __search(key)->val.second; }

    void erase(const TKEY& key) {}
    void erase(const iterator& iter) {}
    void erase(const iterator& beg, const iterator& end) {}

    void insert(const TKEY&, const TVAL&);

    void clear();
    bool empty();
    unsigned int size() { return _size; }

    iterator begin() { return iterator(nil->left); }
    iterator end() { return iterator(nil); }
};

template <typename TKEY, typename TVAL>
map<TKEY, TVAL>::map() : _size(int()) {
    nil = (node_type*)kmalloc(sizeof(node_type));
    root = nil;
    nil->father = nil;
    nil->isRed = false;
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::insert(const TKEY& _key, const TVAL& _val) {
    node_type* _node = (node_type*)kmalloc(sizeof(node_type));
    _node->val = make_pair(_key, _val);
    _node->left = _node->right = nil;
    __insert(_node);
}

//_gnf: if next node is nil then get father node
template <typename TKEY, typename TVAL>
__map_node<TKEY, TVAL>* map<TKEY, TVAL>::__search(TKEY _key, bool _gnf) {
    // FIXME
    if (root == nil)
        return (_gnf ? root : nil);

    node_type* f;
    node_type* _p = this->root;

    do {
        if (_p->val.first == _key)
            return _p;

        f = _p;
        if (_p->val.first > _key)
            _p = _p->left;
        else
            _p = _p->right;
    } while (_p != nil);
    return (_gnf ? f : nil);
}

template <typename TKEY, typename TVAL>
__map_node<TKEY, TVAL>* map<TKEY, TVAL>::__leftmost() {
    node_type* l = root;
    while (l->left != nil)
        l = l->left;
    return l;
}

template <typename TKEY, typename TVAL>
__map_node<TKEY, TVAL>* map<TKEY, TVAL>::__rightmost() {
    node_type* r = root;
    while (r->right != nil)
        r = r->right;
    return r;
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__insert(node_type* _node) {
    // 1.empty tree
    if (!this->_size) {
        root = _node;
        _node->isRed = false;  // root must be black
        _size = 1;
        return;
    }

    // 2.key already exists
    node_type* f = __search(_node->val.first, true);
    if (f->val.first == _node->val.first) {
        f->val.second = _node->val.second;
        return;
    }

    // 3.the f is a black node
    _node->father = f;
    if (f->val.first < _node->val.first)
        f->right = _node;
    else
        f->left = _node;

    // 4.the f is a red node
    if (f->isRed)
        __fixup(_node);

    nil->father = root;
    nil->left = __leftmost();
    nil->right = __rightmost();
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__fixup(node_type* _node) {
    node_type* father_node = _node->father;
    node_type* grandfather_node;
    node_type* uncle_node;

    if (father_node != nil)
        grandfather_node = father_node->father;

    if (grandfather_node != nil)
        if (grandfather_node->left = father_node)
            uncle_node = grandfather_node->right;
        else
            uncle_node = grandfather_node->left;

    // 4-1.uncle node is exists and is red
    if (uncle_node != nil && uncle_node->isRed) {
        uncle_node->isRed = false;
        father_node->isRed = false;
        grandfather_node->isRed = true;
        __fixup(grandfather_node);
    }

    // 4-2.uncle node is not exists or is black
    if (uncle_node == nil || !uncle_node->isRed) {
        // 4-2-1.the father node is the left child of grandfather node
        if (father_node == grandfather_node->left) {
            if (father_node->left == _node) {
                father_node->isRed = false;
                grandfather_node->isRed = true;
                __right_rotate(grandfather_node);
            } else {
                __left_rotate(father_node);
                __fixup(father_node);
            }
        }
        // 4-2-2.the father node is the right child of grandfather node
        else {
            if (father_node->right == _node) {
                father_node->isRed = false;
                grandfather_node->isRed = true;
                __left_rotate(grandfather_node);
            } else {
                __right_rotate(father_node);
                __fixup(father_node);
            }
        }
    }
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__left_rotate(node_type* _node) {
    node_type* r_son = _node->right;

    _node->right = r_son->left;
    if (r_son->left != nil)
        r_son->left->father = _node;

    r_son->father = _node->father;
    if (_node->father == nil)
        this->root = r_son;
    else if (_node == _node->father->left)
        _node->father->left = r_son;
    else
        _node->father->right = r_son;

    r_son->left = _node;
    _node->father = r_son;
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__right_rotate(node_type* _node) {
    node_type* l_son = _node->left;

    _node->left = l_son->right;
    if (l_son->right != nil)
        l_son->right->father = _node;

    l_son->father = _node->father;
    if (_node->father == nil)
        this->root = l_son;
    else if (_node == _node->father->left)
        _node->father->left = l_son;
    else
        _node->father->right = l_son;

    l_son->right = _node;
    _node->father = l_son;
}

#endif
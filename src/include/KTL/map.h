/*
 * @Author: Dizzrt
 * @LastEditTime: 2022-01-03 16:07:37
 */

#ifndef __BIG_MAP_H__
#define __BIG_MAP_H__
#include "MMU\memory.h"
#include "pair.h"

template <typename TKEY, typename TVAL>
struct __map_node {
    bool isRed = true;
    pair<const TKEY, TVAL> val;

    __map_node* left = nullptr;
    __map_node* right = nullptr;
    __map_node* father = nullptr;
};

template <typename TKEY, typename TVAL>
struct __map_iterator {
    typedef __map_node<TKEY, TVAL> node_type;

    node_type* m_node;
    __map_iterator(node_type* _node) : m_node(_node) {
        while (m_node->left != nullptr)
            m_node = m_node->left;
    }

    node_type* getNext() {
        if (m_node == nullptr)
            return nullptr;

        node_type* f = m_node->father;
        if (f == nullptr)
            ;  // TODO no father

        // m_node is left child
        if (f->left == m_node) {
            if (m_node->right == nullptr)
                return m_node->father;
            else {
                node_type* temp = m_node->right;
                while (temp->left != nullptr)
                    temp = temp->left;
                return temp;
            }
        }
        // m_node is right child
        else {
            if (m_node->right != nullptr)
                return m_node->right;
            else {
                // grand father node
                node_type* gf = f->father;
                if (gf == nullptr)
                    ;  // TODO gf is null

                if (gf->left == f)
                    return gf;
                else
                    return nullptr;
            }
        }
    }
    node_type* getPrev() {}
};

template <typename TKEY, typename TVAL>
class map {
  private:
    typedef __map_node<TKEY, TVAL> node_type;

    node_type nil;
    node_type* root;
    // root's left points to the most left node whitch is the first node in LDR
    // root's right points to the most right node whitch is the last node in LDR
    // root's father points to the top node
    unsigned int _size;

    void __insert(node_type*);
    void __remove(node_type*);
    node_type* __search(TKEY, bool = false);

    void __left_rotate(node_type*);
    void __right_rotate(node_type*);

    void __fixup(node_type*);

  public:
    typedef __map_iterator<TKEY, TVAL> iterator;

    map();
    //~map();

    // TVAL& operator[](const TKEY& key) { return __search(key)->val; }

    bool count(const TKEY& key) {}
    TVAL& find(const TKEY& key) {}

    void erase(const TKEY& key) {}
    void erase(const iterator& iter) {}
    void erase(const iterator& beg, const iterator& end) {}

    void insert(const TKEY&, const TVAL&);

    void clear();
    bool empty();

    iterator begin() { return iterator(root->left); }
    iterator end() { return iterator(root->right); }
};
template <typename TKEY, typename TVAL>
map<TKEY, TVAL>::map() : root(&nil), _size(int()) {}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::insert(const TKEY& _key, const TVAL& _val) {
    node_type* _node = (node_type*)kmalloc(sizeof(node_type));
    _node->val = make_pair(_key, _val);
    __insert(_node);
}

template <typename TKEY, typename TVAL>
__map_node<TKEY, TVAL>* map<TKEY, TVAL>::__search(TKEY _key, bool _gnf) {  //_gnf: if nullptr then get father node
    if (root == nullptr)
        return (_gnf ? root : nullptr);

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
    } while (_p != nullptr);
    return (_gnf ? f : nullptr);
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__insert(node_type* _node) {
    // 1.empty tree
    if (!this->_size) {
        root = _node;
        _node->isRed = false;  // new _node must be red,turn it to black
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
}

template <typename TKEY, typename TVAL>
void map<TKEY, TVAL>::__fixup(node_type* _node) {
    node_type* father_node = _node->father;
    node_type* grandfather_node;
    node_type* uncle_node;

    if (father_node != nullptr)
        grandfather_node = father_node->father;

    if (grandfather_node != nullptr)
        if (grandfather_node->left = father_node)
            uncle_node = grandfather_node->right;
        else
            uncle_node = grandfather_node->left;

    // 4-1.uncle node is exists and is red
    if (uncle_node != nullptr && uncle_node->isRed) {
        uncle_node->isRed = false;
        father_node->isRed = false;
        grandfather_node->isRed = true;
        __fixup(grandfather_node);
    }

    // 4-2.uncle node is not exists or is black
    if (uncle_node == nullptr || !uncle_node->isRed) {
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
    if (r_son->left != nullptr)
        r_son->left->father = _node;

    r_son->father = _node->father;
    if (_node->father == nullptr)
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
    if (l_son->right != nullptr)
        l_son->right->father = _node;

    l_son->father = _node->father;
    if (_node->father == nullptr)
        this->root = l_son;
    else if (_node == _node->father->left)
        _node->father->left = l_son;
    else
        _node->father->right = l_son;

    l_son->right = _node;
    _node->father = l_son;
}

#endif
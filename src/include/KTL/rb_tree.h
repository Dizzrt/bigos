#ifndef __BIG_RBTREE_H__
#define __BIG_RBTREE_H__

#include "io.h"
#include "new.h"
#include "pair.h"

template <typename TKEY, typename TVAL>
struct _rb_tree_node {
    bool isRed = true;
    pair<const TKEY, TVAL> val;

    _rb_tree_node* left;
    _rb_tree_node* right;
    _rb_tree_node* father;

    _rb_tree_node() = default;
    _rb_tree_node(const TKEY& key, const TVAL& val) : val(make_pair<const TKEY, TVAL>(key, val)) {}
};

template <typename TKEY, typename TVAL>
class _rb_tree {
  private:
    unsigned int _node_count;

  protected:
    typedef _rb_tree_node<TKEY, TVAL> node_type;

    node_type* nil;
    node_type* root;
    node_type nilNode;

    void __left_rotate(node_type*);
    void __right_rotate(node_type*);
    void __fixup(node_type*);
    void __delfixup(node_type*);

    node_type* __leftmost();
    node_type* __rightmost();

    node_type* __search(TKEY, bool = false);
    node_type* __successor(node_type*);
    node_type* __precursor(node_type*);

  public:
    void __insert(node_type*);
    void __remove(const TKEY&);

    _rb_tree(/* args */);
    ~_rb_tree() = default;

    bool empty() { return nil->left == nil; };
    unsigned int size() { return _node_count; }

    bool count(const TKEY& key) { return __search(key) != nil; };
    TVAL& find(const TKEY& key) { return __search(key)->val.second; };

    node_type* ts() { return root; }  // TEMP
};

template <typename TKEY, typename TVAL>
_rb_tree<TKEY, TVAL>::_rb_tree() {
    root = nil = &nilNode;
    nilNode.isRed = false;
    nil->father = nil->left = nil->right = nil;
}

template <typename TKEY, typename TVAL>
_rb_tree_node<TKEY, TVAL>* _rb_tree<TKEY, TVAL>::__search(TKEY key, bool noNil) {
    // if we find nil and noNil==true,then return the previous node

    if (root == nil)
        return root;

    node_type* previous_node;
    node_type* _node = this->root;

    do {
        if (_node->val.first == key)
            break;

        previous_node = _node;

        if (_node->val.first > key)
            _node = _node->left;
        else
            _node = _node->right;
    } while (_node != nil);

    return (noNil ? previous_node : _node);
}

template <typename TKEY, typename TVAL>
_rb_tree_node<TKEY, TVAL>* _rb_tree<TKEY, TVAL>::__successor(node_type* _node) {
    if (_node->right != nil) {
        _node = _node->right;
        while (_node->left != nil)
            _node = _node->left;

    } else {
        node_type* f = _node->father;
        while (_node == f->right) {
            _node = f;
            f = f->father;
        }
        if (_node->right != f)
            _node = f;
    }

    return _node;
}

template <typename TKEY, typename TVAL>
_rb_tree_node<TKEY, TVAL>* _rb_tree<TKEY, TVAL>::__precursor(node_type* _node) {
    if (_node->isRed && _node->father->father == _node)
        _node = _node->right;
    else if (_node->left != nil) {
        node_type* temp = _node->left;
        while (temp->right != nil)
            temp = temp->right;
        _node = temp;

    } else {
        node_type* f = _node->father;
        while (_node == f->left) {
            _node = f;
            f = f->father;
        }
        _node = f;
    }

    return _node;
}

template <typename TKEY, typename TVAL>
void _rb_tree<TKEY, TVAL>::__left_rotate(node_type* _node) {
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
void _rb_tree<TKEY, TVAL>::__right_rotate(node_type* _node) {
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

template <typename TKEY, typename TVAL>
_rb_tree_node<TKEY, TVAL>* _rb_tree<TKEY, TVAL>::__leftmost() {
    node_type* l = root;

    while (l->left != nil)
        l = l->left;

    return l;
}

template <typename TKEY, typename TVAL>
_rb_tree_node<TKEY, TVAL>* _rb_tree<TKEY, TVAL>::__rightmost() {
    node_type* r = root;

    while (r->right != nil)
        r = r->right;

    return r;
}

template <typename TKEY, typename TVAL>
void _rb_tree<TKEY, TVAL>::__fixup(node_type* _node) {
    if (_node == nil)
        return;

    node_type* father_node = _node->father;
    node_type* grandfather_node = nil;
    node_type* uncle_node = nil;

    if (father_node != nil)
        grandfather_node = father_node->father;
    else
        return;

    if (grandfather_node == nil)
        return;
    else if (grandfather_node->left == father_node)
        uncle_node = grandfather_node->right;
    else
        uncle_node = grandfather_node->left;

    // 4-1.uncle node is exists and is red
    if (uncle_node != nil && uncle_node->isRed) {
        uncle_node->isRed = false;
        father_node->isRed = false;

        if (grandfather_node == root)
            return;

        grandfather_node->isRed = true;
        __fixup(grandfather_node);
        return;
    }
    // 4-2.uncle node is not exists or is black,and the father is left child of grandfather
    else if (grandfather_node->left == father_node) {
        // 4-2-1._node is left child of its father node
        if (father_node->left == _node) {
            father_node->isRed = false;
            grandfather_node->isRed = true;
            __right_rotate(grandfather_node);
            return;
        }
        // 4-2-2._node is right child of its father node
        else {
            __left_rotate(father_node);
            __fixup(father_node);
            return;
        }
    }
    // 4-3.uncle node is not exists or is black,and the father is right child of grandfather
    else {
        // 4-3-1._node is right child of its father node
        if (father_node->right == _node) {
            father_node->isRed = false;
            grandfather_node->isRed = true;
            __left_rotate(grandfather_node);
            return;
        }
        // 4-3-2._node is left child of its father node
        else {
            __right_rotate(father_node);
            __fixup(father_node);
            return;
        }
    }
}

template <typename TKEY, typename TVAL>
void _rb_tree<TKEY, TVAL>::__delfixup(node_type* _node) {}

template <typename TKEY, typename TVAL>
void _rb_tree<TKEY, TVAL>::__insert(node_type* _node) {
    _node->left = _node->right = nil;

    // 1.empty tree
    if (!this->_node_count) {
        _node->isRed = false;  // root must be black
        _node_count = 1;

        _node->father = nil;
        root = nil->father = nil->left = nil->right = _node;
        return;
    }

    node_type* f = __search(_node->val.first, true);

    // 2.key already exists
    if (f->val.first == _node->val.first) {
        f->val.second = _node->val.second;
        return;
    }

    // insert
    _node->father = f;
    if (f->val.first < _node->val.first)
        f->right = _node;
    else
        f->left = _node;
    _node_count++;

    // 3.the f is a black node
    if (!f->isRed)
        return;
    // 4.the f is a red node
    else
        __fixup(_node);

    nil->father = root;
    nil->left = __leftmost();
    nil->right = __rightmost();
}

template <typename TKEY, typename TVAL>
void _rb_tree<TKEY, TVAL>::__remove(const TKEY& key) {
    node_type* t_node = __search(key);
    if (t_node == nil)
        return;  // the target node is not exist

    // 1. has no children
    if (t_node->left == nil && t_node->right == nil) {
        // 1-1. t_node is red
        if (t_node->isRed) {
            if (t_node->father->left == t_node)
                t_node->father->left = nil;
            else
                t_node->father->right = nil;
        }
        // 1-2. t_node is black
        else {
            // TODO delfixup
        }
    }
    // 2. has two children
    else if (t_node->left != nil && t_node->right != nil) {
        node_type* successor = __successor(t_node);

    }
    // 3. has one child
    else {
        node_type* child = t_node->left == nil ? t_node->right : t_node->left;

        if (t_node->father->left == t_node)
            t_node->father->left = child;
        else
            t_node->father->right = child;

        child->isRed = false;
    }
}

#endif
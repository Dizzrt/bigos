#ifndef __BIG_RBTREE_H__
#define __BIG_RBTREE_H__

#include "pair.h"
#include "algorithem.h"

template <typename _Key, typename _Val>
struct _rb_tree_node {
    bool isRed = true;

    pair<const _Key, _Val> val;

    _rb_tree_node* left;
    _rb_tree_node* right;
    _rb_tree_node* father;

    _rb_tree_node(const _Key& _key, const _Val& _val)
        :val(make_pair<const _Key, _Val>(_key, _val)),
        left(nullptr), right(nullptr), father(nullptr) {}
    _rb_tree_node(const _Key& _key) : _rb_tree_node(_key, _Val()) {}
    _rb_tree_node() : _rb_tree_node(_Key(), _Val()) {}
};

template <typename _Key, typename _Val>
_rb_tree_node<_Key, _Val>*
__successor(const _rb_tree_node<_Key, _Val>* _node, const _rb_tree_node<_Key, _Val>* nil) {
    if (_node == nil)
        return nil->left;

    if (_node->right != nil) {
        _node = _node->right;
        while (_node->left != nil)
            _node = _node->left;
    }
    else {
        _rb_tree_node<_Key, _Val>* temp = _node->father;
        while (temp != nil && _node == temp->right)
        {
            _node = temp;
            temp = temp->father;
        }
        _node = temp;
    }

    return const_cast<_rb_tree_node<_Key, _Val>*>(_node);
}

template <typename _Key, typename _Val>
_rb_tree_node<_Key, _Val>*
__precursor(const _rb_tree_node<_Key, _Val>* _node, const _rb_tree_node<_Key, _Val>* nil) {
    if (_node == nil)
        return nil->right;

    _rb_tree_node<_Key, _Val>* temp;
    if (_node->left != nil) {
        temp = _node->left;
        while (temp->right != nil)
            temp = temp->right;
        _node = temp;
    }
    else {
        temp = _node->father;
        while (_node == temp->left) {
            _node = temp;
            temp = temp->father;
        }
        _node = temp;
    }

    return const_cast<_rb_tree_node<_Key, _Val>*>(_node);
}

template <typename _Key, typename _Val>
class _rb_tree;

template <typename _Key, typename _Val>
struct _rb_tree_iterator {
private:
    typedef _rb_tree_node<_Key, _Val> node_type;
    typedef _rb_tree_iterator<_Key, _Val> iterator;

    node_type* nil;
public:
    node_type* m_node;
    _rb_tree_iterator() = default;
    _rb_tree_iterator(node_type* _node, node_type* _nil) :m_node(_node), nil(_nil) {}
    _rb_tree_iterator(node_type* _node) :m_node(_node) {
        if (_node == _node->father->father)
            nil = _node;
        else nil = _node->left;
    }

    iterator operator++(int) {
        iterator temp = *this;
        m_node = __successor(m_node, nil);
        return temp;
    }

    iterator operator++() {
        m_node = __successor(m_node, nil);
        return *this;
    }

    iterator operator--(int) {
        iterator temp = *this;
        m_node = __precursor(m_node, nil);
        return temp;
    }

    iterator operator--() {
        m_node = __precursor(m_node, nil);
        return *this;
    }

    pair<const _Key, _Val>& operator*() const { return m_node->val; }
    friend bool operator==(const iterator& x, const iterator& y) noexcept {
        return x.m_node == y.m_node;
    }
    //bool operator==(const iterator& x) { return this->m_node == x.m_node; }
    bool operator!=(const iterator& x) { return this->m_node != x.m_node; }
    bool operator<(const iterator& x) { return m_node->val < x.m_node->val; }
};

template <typename _Key, typename _Val>
class _rb_tree {
private:
    typedef _rb_tree_node<_Key, _Val> node_type;
    unsigned long __nodeCount;

    void __swapNode(node_type*, node_type*);
protected:
    const unsigned long& nodeCount = __nodeCount;

    node_type* nil;
    node_type* root;
    node_type nilNode;

    void __left_rotate(node_type*);
    void __right_rotate(node_type*);

    void __rebalance(node_type*);
    void __delRebalance(node_type*);

    node_type* __minimum(node_type*);
    node_type* __maximum(node_type*);

    node_type* __search(const _Key&, bool = false);


public:
    void insert(node_type*);
    void remove(node_type*);
    void remove(const _Key&);

    bool empty();
    bool count(const _Key&);

    unsigned int size();
    _Val& find(const _Key&);

    typedef _rb_tree_iterator<_Key, _Val> iterator;

    iterator begin() { return iterator(nil->left); }
    iterator end() { return iterator(nil); }

    _rb_tree();
    ~_rb_tree() = default;
};

template <typename _Key, typename _Val>
_Val& _rb_tree<_Key, _Val>::find(const _Key& key) {
    return __search(key)->val.second;
}

template <typename _Key, typename _Val>
_rb_tree<_Key, _Val>::_rb_tree() {
    nil = &nilNode;
    nil->isRed = false;
    nil->father = nil->left = nil->right = nil;
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::__left_rotate(node_type* _node) {
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

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::__right_rotate(node_type* _node) {
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

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::__rebalance(node_type* _node) {
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
    else uncle_node =
        (grandfather_node->left == father_node ? grandfather_node->right
            : grandfather_node->left);

    // 4-1. uncle node is exists and is read
    if (uncle_node != nil && uncle_node->isRed) {
        uncle_node->isRed = false;
        father_node->isRed = false;

        if (grandfather_node == root)
            return;

        grandfather_node->isRed = true;
        __rebalance(grandfather_node);
        return;
    }
    // 4-2|3. uncle node is not exists or is black
    // 4-2. father node is the left child of grandfather node
    else if (father_node == grandfather_node->left) {
        // 4-2-1. the _node is left child of father
        if (_node == father_node->left) {
            father_node->isRed = false;
            grandfather_node->isRed = true;
            __right_rotate(grandfather_node);
        }
        // 4-2-2. the _node is right child of father
        else {
            __left_rotate(father_node);
            __rebalance(father_node);
        }
    }
    // 4-3. father node is the right child of grandfather node
    else {
        // 4-3-1. the _node is left child of father
        if (_node == father_node->left) {
            __right_rotate(father_node);
            __rebalance(father_node);
        }
        // 4-3-2. the _node is right child of father
        else {
            father_node->isRed = false;
            grandfather_node->isRed = true;
            __left_rotate(grandfather_node);
        }
    }
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::__delRebalance(node_type* _node) {
    if (_node == root)
        return;
    // here => _node must be a black leaf node
    // S => brother node
    // SL => brother node's left child
    // SR => brother node's right child
    // F => father node GF=> grandfather node

    node_type* bro;
    node_type* f = _node->father;

    bool _nodeIsLeft;
    if (f->left == _node)
        _nodeIsLeft = true, bro = f->right;
    else _nodeIsLeft = false, bro = f->left;

    // 1. S is a red node
    if (bro->isRed) {
        f->isRed = true;
        bro->isRed = false;
        // 1-1. _node is left child
        if (_nodeIsLeft) {
            __left_rotate(f);
            __delRebalance(_node);
            return;
        }
        // 1-2. _node is right child
        else {
            __right_rotate(f);
            __delRebalance(_node);
            return;
        }
    }
    // 2. S is a black node
    else {
        // 2-1. _node is left child
        if (_nodeIsLeft) {
            // 2-1-1. the farther nephew is red
            if (bro->right->isRed) {
                swap<bool>(f->isRed, bro->isRed);
                __left_rotate(f);
                bro->right->isRed = false;
                //f->left = nil;
                return;
            }
            // 2-1-2. the nearer nephew is red
            else if (bro->left->isRed) {
                swap<bool>(bro->isRed, bro->left->isRed);
                __right_rotate(bro);
                __delRebalance(_node);
                return;
            }
        }
        // 2-2. _node is right child
        else {
            // 2-2-1. the farther nephew is red
            if (bro->left->isRed) {
                swap<bool>(f->isRed, bro->isRed);
                __right_rotate(f);
                bro->left->isRed = false;
                //f->right = nil;
                return;
            }
            // 2-2-2. the nearer nephew is red
            else if (bro->right->isRed) {
                swap<bool>(bro->isRed, bro->right->isRed);
                __left_rotate(bro);
                __delRebalance(_node);
                return;
            }
        }
    }

    // here => S has no red child
    // 3. F is red
    if (f->isRed) {
        f->isRed = false;
        bro->isRed = true;
    }
    // 4. F is black
    else {
        bro->isRed = true;
        __delRebalance(f);
        return;
    }
}

template <typename _Key, typename _Val>
_rb_tree_node<_Key, _Val>* _rb_tree<_Key, _Val>::__minimum(node_type* _node) {
    while (_node->left != nil)
        _node = _node->left;

    return _node;
}

template <typename _Key, typename _Val>
_rb_tree_node<_Key, _Val>* _rb_tree<_Key, _Val>::__maximum(node_type* _node) {
    while (_node->right != nil)
        _node = _node->right;

    return _node;
}

template <typename _Key, typename _Val>
_rb_tree_node<_Key, _Val>* _rb_tree<_Key, _Val>::__search(const _Key& _t, bool noNil) {
    if (root == nil)
        return root;

    node_type* pnode;         // previous node
    node_type* cnode = root;  // current ndoe;

    do {
        if (cnode->val.first == _t)
            break;

        pnode = cnode;

        if (cnode->val.first > _t)
            cnode = cnode->left;
        else
            cnode = cnode->right;
    } while (cnode != nil);

    return (noNil ? pnode : cnode);
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::insert(node_type* _node) {
    _node->left = _node->right = nil;

    // 1. empty tree
    if (this->empty()) {
        _node->isRed = false;
        __nodeCount = 1;

        _node->father = nil;
        root = nil->father = nil->left = nil->right = _node;
        return;
    }

    node_type* f = __search(_node->val.first, true);

    // 2. key already exists
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
    __nodeCount++;

    // 3. f is a black node, do nothing
    // 4. f is a red node, need rebalance
    if (f->isRed)
        __rebalance(_node);

    nil->father = root;
    nil->left = __minimum(root);
    nil->right = __maximum(root);
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::__swapNode(node_type* _node, node_type* _suc) {
    if (_node == root) {
        root = _suc;
        nil->father = _suc;
    }
    else if (_node == _node->father->left)
        _node->father->left = _suc;
    else _node->father->right = _suc;

    if (_suc == _node->right) {
        _suc->father = _node->father;
        _node->father = _suc;
        _node->right = _suc->right;
        _suc->right = _node;
    }
    else {
        if (_suc == _suc->father->left)
            _suc->father->left = _node;
        else _suc->father->right = _node;

        swap<node_type*>(_node->father, _suc->father);
        swap<node_type*>(_node->right, _suc->right);

        if (_suc->right != nil)
            _suc->right->father = _suc;
    }

    _suc->left = _node->left;
    if (_suc->left != nil)
        _suc->left->father = _suc;
    _node->left = nil;

    if (_node->right != nil)
        _node->right->father = _node;

    swap<bool>(_node->isRed, _suc->isRed);
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::remove(const _Key& key) {
    node_type* _node = __search(key);
    if (_node == nil)
        return;

    remove(_node);
}

template <typename _Key, typename _Val>
void _rb_tree<_Key, _Val>::remove(node_type* _node) {
    if (_node->left != nil && _node->right != nil)
        __swapNode(_node, __successor(_node, nil));

    if (_node->isRed)
    {
        if (_node->father->left == _node)
            _node->father->left = nil;
        else _node->father->right = nil;
    }
    // at here => _node is a black node, has no more than one child,
    // and the only child must be a red node
    else if (_node->left != nil) {
        if (_node->father->left == _node)
            _node->father->left = _node->left;
        else _node->father->right = _node->left;

        _node->left->father = _node->father;
        _node->left->isRed = _node->isRed;
    }
    else if (_node->right != nil) {
        if (_node->father->left == _node)
            _node->father->left = _node->right;
        else _node->father->right = _node->right;

        _node->right->father = _node->father;
        _node->right->isRed = _node->isRed;
    }
    else {
        __delRebalance(_node);
        if (_node == _node->father->left)
            _node->father->left = nil;
        else _node->father->right = nil;
    }

    //if _node still the root after rebalance, then _node must the last node of this tree
    if (_node == root)
        root = nil;

    __nodeCount--;

    nil->father = root;
    nil->left = __minimum(root);
    nil->right = __maximum(root);
}

template <typename _Key, typename _Val>
bool _rb_tree<_Key, _Val>::empty() {
    return nil->left == nil;
}

template <typename _Key, typename _Val>
bool _rb_tree<_Key, _Val>::count(const _Key& _t) {
    return __search(_t) != nil;
}

template <typename _Key, typename _Val>
unsigned int _rb_tree<_Key, _Val>::size() {
    return nodeCount;
}

#endif
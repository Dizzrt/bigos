#ifndef __BIG_RBTREE_H__
#define __BIG_RBTREE_H__

template <typename _Key, typename _Val>
struct _rb_tree_node {
    bool isRed = true;

    const _Key key;
    _Val val;

    _rb_tree_node* left;
    _rb_tree_node* right;
    _rb_tree_node* father;

    _rb_tree_node(const _Key& _key, const _Val _val) : key(_key), val(_val) {}
    _rb_tree_node(const _Key& _key) : _rb_tree_node(_key, _Val()) {}
    _rb_tree_node() :_rb_tree_node(_Key(), _Val()) {}
};

template <typename _Key, typename _Val>
class _rb_tree;

template <typename _Key, typename _Val>
struct _rb_tree_iterator {
private:
    typedef _rb_tree_node<_Key, _Val> node_type;
    typedef _rb_tree_iterator<_Key, _Val> iterator;

    node_type* nil;
    void __precursor();
    void __successor();
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
        __successor();
        return temp;
    }

    iterator operator++() {
        __successor();
        return *this;
    }

    iterator operator--(int) {
        iterator temp = *this;
        __precursor();
        return temp;
    }

    iterator operator--() {
        __precursor();
        return *this;
    }

    _Val operator*() const { return m_node->val; }
    bool operator==(const iterator& x) { return this->m_node == x.m_node; }
    bool operator!=(const iterator& x) { return this->m_node != x.m_node; }
    bool operator<(const iterator& x) { return m_node->val < x.m_node->val; }
};

template <typename _Key, typename _Val>
void _rb_tree_iterator<_Key, _Val>::__successor() {
    if (m_node == nil)
        m_node = nil->left;
    else if (m_node->right != nil) {
        m_node = m_node->right;
        while (m_node->left != nil)
            m_node = m_node->left;
    }
    else {
        node_type* f = m_node->father;
        while (f != nil && m_node == f->right) {
            m_node = f;
            f = f->father;
        }
        m_node = f;
    }
}

template <typename _Key, typename _Val>
void _rb_tree_iterator<_Key, _Val>::__precursor() {
    if (m_node == nil) { // end()
        m_node = nil->right;
        return;
    }

    if (m_node->left != nil) {
        node_type* temp = m_node->left;
        while (temp->right != nil)
            temp = temp->right;
        m_node = temp;
    }
    else {
        node_type* temp = m_node->father;
        while (m_node == temp->left) {
            m_node = temp;
            temp = temp->father;
        }
        m_node = temp;
    }
}

template <typename _Key, typename _Val>
class _rb_tree {
private:
    typedef _rb_tree_node<_Key, _Val> node_type;
    unsigned long __nodeCount;

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
    void remove(const _Key&);

    bool empty();
    bool count(const _Key&);

    unsigned int size();

    typedef _rb_tree_iterator<_Key, _Val> iterator;

    iterator begin() { return iterator(nil->left); }
    iterator end() { return iterator(nil); }

    _rb_tree();
    ~_rb_tree() = default;
};

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
    //TODO delRebalance
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
_rb_tree_node<_Key, _Val>* _rb_tree<_Key, _Val>::__search(const _Key& _t,
    bool noNil) {
    if (root == nil)
        return root;

    node_type* pnode;         // previous node
    node_type* cnode = root;  // current ndoe;

    do {
        if (cnode->key == _t)
            break;

        pnode = cnode;

        if (cnode->key > _t)
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

    node_type* f = __search(_node->key, true);

    // 2. key already exists
    if (f->key == _node->key) {
        f->val = _node->val;
        return;
    }

    // insert
    _node->father = f;
    if (f->key < _node->key)
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
void _rb_tree<_Key, _Val>::remove(const _Key&) {
    //TODO remove
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
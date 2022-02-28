#ifndef __BIG_RBTREE_H__
#define __BIG_RBTREE_H__

template <typename T>
struct _rb_tree_node {
    bool isRed = false;
    T val;

    _rb_tree_node* left;
    _rb_tree_node* right;
    _rb_tree_node* father;

    _rb_tree_node() = default;
    _rb_tree_node(const T& t) : val(t) {}
};

template <typename T>
class _rb_tree {
  private:
    typedef _rb_tree_node<T> node_type;

  protected:
    unsigned int nodeCount;

    node_type* nil;
    node_type* root;
    node_type nilNode;

    void __left_rotate(node_type*);
    void __right_rotate(node_type*);

    void __fixup(node_type*);
    void __delfixup(node_type*);

    node_type* __search(T, bool = false);
    node_type* __leftmost();
    node_type* __rightmost();
    node_type* __successor();
    node_type* __precursor();

  public:
    void insert(node_type*);
    void remove(const T&);

    bool empty();
    bool count(const T&);

    unsigned int size();

    _rb_tree(){};
    ~_rb_tree(){};
};

#endif
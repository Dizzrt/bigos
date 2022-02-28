#ifndef __BIG_KMAP_H__
#define __BIG_KMAP_H__

#include "pair.h"
#include "rb_tree.h"

template <typename KEY, typename VAL>
struct _kmap_node {
    pair<const KEY, VAL> val;

    _kmap_node() = default;
    _kmap_node(const KEY& _key, const VAL& _val) : val(make_pair<const KEY, VAL>(_key, _val)) {}
};

template <typename KEY, typename VAL>
class kmap : public _rb_tree<_kmap_node<KEY, VAL>> {
  private:
    typedef _kmap_node<KEY, VAL> node_type;

  public:
    kmap(/* args */) = default;
    ~kmap() = default;
};

#endif
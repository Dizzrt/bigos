/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-04 20:50:23
 */

#include "MMU\slab.h"

Slab *Slab_cache::rm_from_(list<Slab *> _list, Slab *_tar) {
    _list.remove(_tar);
    return _tar;
}

void Slab_cache::append(list<Slab *> _list, Slab *_tar) { _list.push_back(_tar); }
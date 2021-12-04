/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-04 21:30:23
 */

#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "bitmap.h"
#include "list.h"
#include "stdint.h"

struct Slab {
    uint32_t __free;
    uint32_t __using;

    void *vaddr;
    BitMap bitmap;
};

struct Slab_cache {
    list<Slab *> pool_full;
    list<Slab *> pool_empty;
    list<Slab *> pool_partial;

    void append(list<Slab *>, Slab *);
    Slab *rm_from_(list<Slab *>, Slab *);
    void *__alloc(size_t len) {
        if (len > 0x2000)
            return nullptr;

        if (pool_partial.size() > 0) {
            //__list_node<Slab *> =pool_partial.

        } else if (pool_empty.size() > 0) {

        } else {

            // get new page as cache from buddy system
        }
    }
};

#endif
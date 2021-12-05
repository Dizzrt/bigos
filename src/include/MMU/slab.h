/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-05 16:33:10
 */

#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "bitmap.h"
#include "list.h"
#include "stdint.h"

enum PoolType { FULL, EMPTY, PARTIAL };

struct Slab {
    uint32_t __free;
    uint32_t __using;

    void *vaddr;
    BitMap bitmap;
};

struct Slab_cache {

    void *__alloc(size_t);
    void appendSlab(Slab *);
    void __appendSlab_(__list_node<Slab *> *, PoolType = PoolType::EMPTY);

  private:
    list<Slab *> pool_full;
    list<Slab *> pool_empty;
    list<Slab *> pool_partial;
};

#endif
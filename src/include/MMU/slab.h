/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-12 17:42:52
 */

#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "bitmap.h"
#include "list.h"
#include "stdint.h"

#define SLAB_PERMANENT 0b10000000

// every slab have 4096(1 page) bytes
struct Slab {
    uint8_t flags;
    uint16_t objFree;
    uint32_t objSize;
    uint64_t vaddr;
    BitMap bitmap;

    Slab();
    Slab(uint8_t, uint32_t, uint64_t, uint8_t *);
};

struct Slab_cache {
    void *__alloc(uint16_t);

    list<Slab *> slabs_full;
    list<Slab *> slabs_available;
};

#endif
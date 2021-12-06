/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 20:47:39
 */

#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "bitmap.h"
#include "list.h"
#include "stdint.h"

enum SlabType { NORMAL, PERMANENT };

// every slab have 4096(1 page) bytes to allocate

struct Slab {
    SlabType type;

    BitMap bitmap;
    uint64_t vaddr;

    uint16_t __free;
    uint16_t __using;

    Slab();

    /**
     * @param type(SlabType)  the type of this slab
     * @param vaddr(uint64) the virtual address base of this slab
     * @param bp(uint8*) the pointer of bitmap
     */
    Slab(SlabType, uint64_t, uint8_t *);
};

struct Slab_cache {

    void *__alloc(size_t);
    void appendSlab(Slab *);
    void __appendSlab_(__list_node<Slab *> *);

    // MARKER temporary crtbegin
    void __tmporary_init();

  private:
    list<Slab *> pool_full;
    list<Slab *> pool_available;
};

#endif
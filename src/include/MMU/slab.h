#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "KTL\list.h"
#include "bitmap.h"
#include "kmem.h"

#define SLAB_PERMANENT 0b10000000

// every slab have 4096(1 page) bytes
struct Slab {
    uint8_t flags;
    BitMap bitmap;
    uint64_t vaddr;
    uint16_t objFree;

    Slab();
    Slab(uint8_t, uint32_t, uint64_t, uint8_t*);
};

struct Slab_cache {
    uint32_t objSize;
    list<Slab*> slabs_full;
    list<Slab*> slabs_available;

    void* __alloc(uint16_t, bool = false);
    void __free(Slab*, uint64_t, uint16_t len);
};

struct SlabAllocHeader {
    Slab* slab;
    uint16_t len;
    const char magic[6] = "_SLBA";
};

extern Slab iSlab_0;
extern Slab iSlab_1;

extern linked_container<Slab*> iSlab_lnode_0;
extern linked_container<Slab*> iSlab_lnode_1;

extern Slab_cache common_cache;
extern Slab_cache __Slab_cache;

void __slab_free(Alloc_Header*);

Slab* alloc_slab(uint8_t, uint32_t);

#endif
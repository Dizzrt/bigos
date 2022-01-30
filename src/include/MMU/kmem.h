#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "buddy.h"
#include "slab.h"
#include "stdint.h"

// virtual address to physical address
// void* v2p(void*);
// void* v2p(uint64_t);

// void* p2v(void*);
// void* p2v(uint64_t);

extern Cache slab_cache;
extern Cache buddy_cache;
extern Cache common_cache;
extern Cache buddyMapNode_cache;

struct AMS  // available memory segment
{
    uint64_t base;
    uint64_t len;
};

struct Alloc_Header {
    bool isPages;
    uint32_t len;

    // no use,if pages == true
    uint64_t offset;
    void* _allocator;
};

void kmeminit();
// extern map<const void*, Alloc_Header*> alloc_mp;

#endif
#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

// #include "KTL\list.h"
// #include "KTL\map.h"
#include "buddy.h"
#include "slab.h"
#include "stdint.h"

// virtual address to physical address
// void* v2p(void*);
// void* v2p(uint64_t);

// void* p2v(void*);
// void* p2v(uint64_t);

extern Cache common_cache;
extern linked_container<Slab> common_cache_slab_node_0;
extern linked_container<Slab> common_cache_slab_node_1;

extern Cache buddy_cache;
extern linked_container<Slab> buddy_cache_slab_node_0;
// extern linked_container<Slab*> buddy_cache_slab_node_1;

extern Cache slab_cache;
extern linked_container<Slab> slab_cache_slab_node_0;
extern linked_container<Slab> slab_cache_slab_node_1;

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
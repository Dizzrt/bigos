#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "KTL\map.h"
#include "stdint.h"

// virtual address to physical address
// void* v2p(void*);
// void* v2p(uint64_t);

// void* p2v(void*);
// void* p2v(uint64_t);

struct Alloc_Header {
    bool isPages;
    uint32_t len;

    // no use,if pages == true
    uint64_t offset;
    void* _allocator;
};

extern map<const void*, Alloc_Header*> alloc_mp;

#endif
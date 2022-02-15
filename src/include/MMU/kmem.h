#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "buddy.h"
#include "slab.h"
#include "stdint.h"

extern Cache cache_16;
extern Cache cache_32;
extern Cache cache_64;
extern Cache cache_128;
extern Cache cache_256;
extern Cache cache_512;
extern Cache cache_1024;
extern Cache cache_2048;

extern CacheChain kmem_cache;

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
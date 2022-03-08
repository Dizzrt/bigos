#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "mmu\slab.h"
#include "mmu\buddy.h"

#define SLAB_OBJ_CNT(OBJSIZE) 0x1000 / LONG_ALIGN(OBJSIZE + SHSIZE)
#define SLAB_BP_SIZE(OBJSIZE) SLAB_OBJ_CNT(OBJSIZE) / 8 + (SLAB_OBJ_CNT(OBJSIZE) % 8 == 0 ? 0 : 1)
#define STATIC_SLAB(NAME, FLAGS, OBJSIZE)                                                       \
    static uint8_t ___slabPage_##NAME[0x1000];                                                  \
    static uint8_t ___slabBP_##NAME[SLAB_BP_SIZE(OBJSIZE)];                                     \
    static Slab ___slab_##NAME(FLAGS, OBJSIZE, (uint64_t)___slabPage_##NAME, ___slabBP_##NAME); \
    static linked_container<Slab*> ___slabLC_##NAME(&___slab_##NAME);
#define CACHE(NAME, FLAGS, OBJSIZE, ISLABSIZE, ...)               \
    Cache cache_##NAME(FLAGS, OBJSIZE, ISLABSIZE, ##__VA_ARGS__); \
    static linked_container<Cache*> ___cacheLC_##NAME(&cache_##NAME);

extern CacheChain kmem_cache;

_rb_buddyBlock* getRB_buddyBlock();
void kmemInit();


#endif
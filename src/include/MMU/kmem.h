#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "mmu\slab.h"

#define SLAB_OBJ_CNT(OBJSIZE) 0x1000 / LONG_ALIGN(OBJSIZE + SHSIZE)
#define SLAB_BP_SIZE(OBJSIZE) SLAB_OBJ_CNT(OBJSIZE) / 8 + (SLAB_OBJ_CNT(OBJSIZE) % 8 == 0 ? 0 : 1)
#define STATIC_SLAB_FLAGS(NAME, OBJSIZE, FLAGS)             \
    static uint8_t ___slabPage_##NAME[0x1000];              \
    static uint8_t ___slabBP_##NAME[SLAB_BP_SIZE(OBJSIZE)]; \
    static Slab slab_##NAME(FLAGS, OBJSIZE, (uint64_t)___slabPage_##NAME, ___slabBP_##NAME);
#define STATIC_SLAB(NAME, OBJSIZE) STATIC_SLAB_FLAGS(NAME, OBJSIZE, 128)

#define CACHE(NAME, OBJSIZE) Cache cache_##NAME(OBJSIZE);

void kmemInit();

#endif
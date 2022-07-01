#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "buddy.h"
#include "slab.h"

// //how many objs which size is OBJSIZE in a slab with a capacity of CAPACITY
// #define SLAB_OBJ_CNT(CAPACITY,OBJSIZE) \
//     CAPACITY / LONG_ALIGN(OBJSIZE + SHSIZE)

// //BP => bitmap
// #define SLAB_BP_SIZE(CAPACITY,OBJSIZE) \
//     SLAB_OBJ_CNT(CAPACITY,OBJSIZE) / 8 + (SLAB_OBJ_CNT(CAPACITY,OBJSIZE) % 8 == 0 ? 0 : 1)

// #define STATIC_SLAB(NAME,FLAGS,OBJSIZE,PAGECNT)                                 \
//     static uint8_t __slabPage_##NAME[0x1000*PAGECNT];                           \
//     static uint8_t __slabBP_##NAME[SLAB_BP_SIZE(0x1000*PAGECNT,OBJSIZE)];       \
//     static Slab __slab_##NAME(FLAGS,                                            \
//     SLAB_OBJ_CNT(0x1000 * PAGECNT, OBJSIZE),                                    \
//     LONG_ALIGN(OBJSIZE), (uint64_t)__slabPage_##NAME, __slabBP_##NAME);         \
//     static linked_container<Slab*> __slabLC_##NAME(&__slab_##NAME);

// //SSCNT => Static Slab cnt
// #define STATIC_CACHE(NAME,FLAGS,OBJSIZE,SSIZE,SSCNT,...)                        \
//     Cache Cache_##NAME(FLAGS,OBJSIZE,SSIZE,SSCNT,##__VA_ARGS__);                \
//     static linked_container<Cache*> cacheLC_##NAME(&Cache_##NAME);

#define CACHE_STATIC(NAME,OBJSIZE,FLAGS,SLABORDER,SSC,...)          \
    Cache cache_##NAME(OBJSIZE,FLAGS,SLABORDER,SSC,##__VA_ARGS__);  \
    static linked_container<Cache*> cacheLC_##NAME(&cache_##NAME);

//how many objs when obj size is OBJSIZE and slab order is SLABORDER
#define SLAB_OBJ_CNT(SLABORDER,OBJSIZE)     \
    buddyChunkSize[SLABORDER]/LONG_ALIGN(OBJSIZE+SHSIZE)

#define SLAB_BP_SIZE(SLABORDER,OBJSIZE)     \
    SLAB_OBJ_CNT(SLABORDER,OBJSIZE) / 8 + (SLAB_OBJ_CNT(SLABORDER,OBJSIZE) % 8 == 0 ? 0 : 1)

#define SLAB_STATIC(NAME,SLABORDER,OBJSIZE,FLAGS)                                                 \
    static uint8_t __slabMem_##NAME[buddyChunkSize[SLABORDER]];                                         \
    static uint8_t __slabBP_##NAME[SLAB_BP_SIZE(SLABORDER,OBJSIZE)];                                    \
    static Slab __slab_##NAME(SLABORDER,SLAB_OBJ_CNT(SLABORDER,OBJSIZE),LONG_ALIGN(OBJSIZE+SHSIZE)      \
    ,FLAGS,(uint64_t)__slabMem_##NAME,__slabBP_##NAME);                                                 \
    static linked_container<Slab*> __slabLC_##NAME(&__slab_##NAME);


//initialize slab and buddy
extern void kmem_init();


#endif //__BIG_KMEM_H__
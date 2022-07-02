#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "slab.h"
#include "buddy.h"
#include "ktl\pair.h"

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

#define SLABDELTA(SLABORDER,OBJSIZE)                            \
    (buddyChunkSize[SLABORDER]-(buddyChunkSize[SLABORDER]       \
    /LONG_ALIGN(OBJSIZE+SHSIZE)*LONG_ALIGN(OBJSIZE+SHSIZE)))

#define __SLAB_ORDER_COM_1(OBJSIZE)                               \
    SLABDELTA(1,OBJSIZE)<SLABDELTA(0,OBJSIZE)?                  \
    (SLABDELTA(1,OBJSIZE)<<4)+1:(SLABDELTA(0,OBJSIZE)<<4)+1     \

#define __SLAB_ORDER_COM_2(OBJSIZE)                               \
    SLABDELTA(2,OBJSIZE)<(__SLAB_ORDER_COM_1(OBJSIZE))>>4?        \
    (SLABDELTA(2,OBJSIZE)<<4)+2:__SLAB_ORDER_COM_1(OBJSIZE)       \

#define __SLAB_ORDER_COM_3(OBJSIZE)                               \
    SLABDELTA(3,OBJSIZE)<(__SLAB_ORDER_COM_2(OBJSIZE))>>4?        \
    (SLABDELTA(3,OBJSIZE)<<4)+3:__SLAB_ORDER_COM_2(OBJSIZE)       \

#define __SLAB_ORDER_COM_4(OBJSIZE)                               \
    SLABDELTA(4,OBJSIZE)<(__SLAB_ORDER_COM_3(OBJSIZE))>>4?        \
    (SLABDELTA(4,OBJSIZE)<<4)+4:__SLAB_ORDER_COM_3(OBJSIZE)       \

#define __SLAB_ORDER_COM_5(OBJSIZE)                               \
    SLABDELTA(5,OBJSIZE)<(__SLAB_ORDER_COM_4(OBJSIZE))>>4?        \
    (SLABDELTA(5,OBJSIZE)<<4)+5:__SLAB_ORDER_COM_4(OBJSIZE)       \

#define __SLAB_ORDER_COM_6(OBJSIZE)                               \
    SLABDELTA(6,OBJSIZE)<(__SLAB_ORDER_COM_5(OBJSIZE))>>4?        \
    (SLABDELTA(6,OBJSIZE)<<4)+6:__SLAB_ORDER_COM_5(OBJSIZE)       \

#define __SLAB_ORDER_COM_7(OBJSIZE)                               \
    SLABDELTA(7,OBJSIZE)<(__SLAB_ORDER_COM_6(OBJSIZE))>>4?        \
    (SLABDELTA(7,OBJSIZE)<<4)+7:__SLAB_ORDER_COM_6(OBJSIZE)       \

#define __SLAB_ORDER_COM_8(OBJSIZE)                               \
    SLABDELTA(8,OBJSIZE)<(__SLAB_ORDER_COM_7(OBJSIZE))>>4?        \
    (SLABDELTA(8,OBJSIZE)<<4)+8:__SLAB_ORDER_COM_7(OBJSIZE)       \

#define __SLAB_ORDER_COM_9(OBJSIZE)                               \
    SLABDELTA(9,OBJSIZE)<(__SLAB_ORDER_COM_8(OBJSIZE))>>4?        \
    (SLABDELTA(9,OBJSIZE)<<4)+9:__SLAB_ORDER_COM_8(OBJSIZE)       \

#define __SLAB_ORDER_COM_10(OBJSIZE)                              \
    SLABDELTA(10,OBJSIZE)<(__SLAB_ORDER_COM_9(OBJSIZE))>>4?       \
    (SLABDELTA(10,OBJSIZE)<<4)+10:__SLAB_ORDER_COM_9(OBJSIZE)     \

#define SLAB_ORDER(OBJSIZE)     \
    (__SLAB_ORDER_COM_10(OBJSIZE))&0xf




    //initialize slab and buddy
extern void kmem_init();


void* test();

#endif //__BIG_KMEM_H__
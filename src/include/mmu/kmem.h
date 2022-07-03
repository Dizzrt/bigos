#ifndef __BIG_KMEM_H__
#define __BIG_KMEM_H__

#include "slab.h"
#include "buddy.h"
#include "ktl\pair.h"

#define CACHE_STATIC(NAME,OBJSIZE,FLAGS,SSC,...)          \
    Cache cache_##NAME(OBJSIZE,FLAGS,SLAB_ORDER(OBJSIZE),SSC,##__VA_ARGS__);  \
    static linked_container<Cache*> cacheLC_##NAME(&cache_##NAME);

#define CACHE_ORDER_STATIC(NAME,OBJSIZE,FLAGS,SLABORDER,SSC,...)          \
    Cache cache_##NAME(OBJSIZE,FLAGS,SLABORDER,SSC,##__VA_ARGS__);  \
    static linked_container<Cache*> cacheLC_##NAME(&cache_##NAME);

//how many objs when obj size is OBJSIZE and slab order is SLABORDER
#define SLAB_OBJ_CNT(SLABORDER,OBJSIZE)     \
    buddyChunkSize[SLABORDER]/LONG_ALIGN(OBJSIZE+SHSIZE)

#define SLAB_BP_SIZE(SLABORDER,OBJSIZE)     \
    SLAB_OBJ_CNT(SLABORDER,OBJSIZE) / 8 + (SLAB_OBJ_CNT(SLABORDER,OBJSIZE) % 8 == 0 ? 0 : 1)

#define SLAB_STATIC(NAME,OBJSIZE,FLAGS)                                                 \
    static uint8_t __slabMem_##NAME[buddyChunkSize[SLAB_ORDER(OBJSIZE)]];                                         \
    static uint8_t __slabBP_##NAME[SLAB_BP_SIZE(SLAB_ORDER(OBJSIZE),OBJSIZE)];                                    \
    static Slab __slab_##NAME(SLAB_ORDER(OBJSIZE),SLAB_OBJ_CNT(SLAB_ORDER(OBJSIZE),OBJSIZE),LONG_ALIGN(OBJSIZE+SHSIZE)      \
    ,FLAGS,(uint64_t)__slabMem_##NAME,__slabBP_##NAME);                                                 \
    static linked_container<Slab*> __slabLC_##NAME(&__slab_##NAME);

#define SLAB_ORDER_STATIC(NAME,SLABORDER,OBJSIZE,FLAGS)                                                 \
    static uint8_t __slabMem_##NAME[buddyChunkSize[SLABORDER]];                                         \
    static uint8_t __slabBP_##NAME[SLAB_BP_SIZE(SLABORDER,OBJSIZE)];                                    \
    static Slab __slab_##NAME(SLABORDER,SLAB_OBJ_CNT(SLABORDER,OBJSIZE),LONG_ALIGN(OBJSIZE+SHSIZE)      \
    ,FLAGS,(uint64_t)__slabMem_##NAME,__slabBP_##NAME);                                                 \
    static linked_container<Slab*> __slabLC_##NAME(&__slab_##NAME);

template<uint32_t SlabOrder, uint32_t ObjSize>
struct SlabDelta
{
    static constexpr uint32_t
        _delta = buddyChunkSize[SlabOrder] - buddyChunkSize[SlabOrder]
        / LONG_ALIGN(ObjSize + SHSIZE) * LONG_ALIGN(ObjSize + SHSIZE);
};

#define SLABDELTA(SLABORDER,OBJSIZE)                              \
    SlabDelta<SLABORDER,OBJSIZE>::_delta

template<uint32_t ObjSize, uint32_t CurOrder, uint32_t ...Orders>
struct __Slab_Order {
    static constexpr uint32_t
        _order = SlabDelta<CurOrder, ObjSize>::_delta <
        SlabDelta<__Slab_Order<ObjSize, Orders...>::_order, ObjSize>::_delta ?
        CurOrder : __Slab_Order<ObjSize, Orders...>::_order;
};

template<uint32_t ObjSize, uint32_t _1, uint32_t _0>
struct __Slab_Order<ObjSize, _1, _0>
{
    static constexpr uint32_t
        _order = SlabDelta<_1, ObjSize>::_delta < SlabDelta<_0, ObjSize>::_delta ?
        _1 : _0;
};

#define SLAB_ORDERS 10,9,8,7,6,5,4,3,2,1,0
#define SLAB_ORDER(OBJSIZE)     \
    (__Slab_Order<OBJSIZE,10,SLAB_ORDERS>::_order)


// #define __SLAB_ORDER_COM_1(OBJSIZE)                               \
//     SLABDELTA(1,OBJSIZE)<SLABDELTA(0,OBJSIZE)?                    \
//     (SLABDELTA(1,OBJSIZE)<<4)+1:(SLABDELTA(0,OBJSIZE)<<4)+1       \

// #define __SLAB_ORDER_COM_2(OBJSIZE)                               \
//     SLABDELTA(2,OBJSIZE)<(__SLAB_ORDER_COM_1(OBJSIZE))>>4?        \
//     (SLABDELTA(2,OBJSIZE)<<4)+2:__SLAB_ORDER_COM_1(OBJSIZE)       \

// #define __SLAB_ORDER_COM_3(OBJSIZE)                               \
//     SLABDELTA(3,OBJSIZE)<(__SLAB_ORDER_COM_2(OBJSIZE))>>4?        \
//     (SLABDELTA(3,OBJSIZE)<<4)+3:__SLAB_ORDER_COM_2(OBJSIZE)       \

// #define __SLAB_ORDER_COM_4(OBJSIZE)                               \
//     SLABDELTA(4,OBJSIZE)<(__SLAB_ORDER_COM_3(OBJSIZE))>>4?        \
//     (SLABDELTA(4,OBJSIZE)<<4)+4:__SLAB_ORDER_COM_3(OBJSIZE)       \

// #define __SLAB_ORDER_COM_5(OBJSIZE)                               \
//     SLABDELTA(5,OBJSIZE)<(__SLAB_ORDER_COM_4(OBJSIZE))>>4?        \
//     (SLABDELTA(5,OBJSIZE)<<4)+5:__SLAB_ORDER_COM_4(OBJSIZE)       \

// #define __SLAB_ORDER_COM_6(OBJSIZE)                               \
//     SLABDELTA(6,OBJSIZE)<(__SLAB_ORDER_COM_5(OBJSIZE))>>4?        \
//     (SLABDELTA(6,OBJSIZE)<<4)+6:__SLAB_ORDER_COM_5(OBJSIZE)       \

// #define __SLAB_ORDER_COM_7(OBJSIZE)                               \
//     SLABDELTA(7,OBJSIZE)<(__SLAB_ORDER_COM_6(OBJSIZE))>>4?        \
//     (SLABDELTA(7,OBJSIZE)<<4)+7:__SLAB_ORDER_COM_6(OBJSIZE)       \

// #define __SLAB_ORDER_COM_8(OBJSIZE)                               \
//     SLABDELTA(8,OBJSIZE)<(__SLAB_ORDER_COM_7(OBJSIZE))>>4?        \
//     (SLABDELTA(8,OBJSIZE)<<4)+8:__SLAB_ORDER_COM_7(OBJSIZE)       \

// #define __SLAB_ORDER_COM_9(OBJSIZE)                               \
//     SLABDELTA(9,OBJSIZE)<(__SLAB_ORDER_COM_8(OBJSIZE))>>4?        \
//     (SLABDELTA(9,OBJSIZE)<<4)+9:__SLAB_ORDER_COM_8(OBJSIZE)       \

// #define __SLAB_ORDER_COM_10(OBJSIZE)                              \
//     SLABDELTA(10,OBJSIZE)<(__SLAB_ORDER_COM_9(OBJSIZE))>>4?       \
//     (SLABDELTA(10,OBJSIZE)<<4)+10:__SLAB_ORDER_COM_9(OBJSIZE)     \

// #define SLAB_ORDER(OBJSIZE)     \
//     (__SLAB_ORDER_COM_10(OBJSIZE))&0xf


extern void kmem_init();


#endif //__BIG_KMEM_H__
#include "mmu\kmem.h"
#include "io.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(lcPointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(lcPointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))
// STATIC_SLAB(RB_buddyBlock_0, SLAB_PERMANENT, RB_BUDDY_SIZE)

STATIC_SLAB(8B, SLAB_PERMANENT, 8)
STATIC_SLAB(16B, SLAB_PERMANENT, 16)
STATIC_SLAB(32B, SLAB_PERMANENT, 32)
STATIC_SLAB(64B, SLAB_PERMANENT, 64)
STATIC_SLAB(128B, SLAB_PERMANENT, 128)
STATIC_SLAB(256B, SLAB_PERMANENT, 256)
STATIC_SLAB(512B, SLAB_PERMANENT, 512)
STATIC_SLAB(1024B, SLAB_PERMANENT, 1024)

CACHE(slab, CACHE_NONEMPTY, sizeof(Slab), 2, &___slabLC_slab_0, &___slabLC_slab_1)
CACHE(lcPointer, CACHE_NONEMPTY, sizeof(linked_container<void*>), 2, &___slabLC_lcPointer_0, &___slabLC_lcPointer_1)
// CACHE(RB_buddyBlock, 0, RB_BUDDY_SIZE, 1, &___slab_RB_buddyBlock_0);

CACHE(8B, 0, 8, 1, &___slab_8B)
CACHE(16B, 0, 16, 1, &___slab_16B)
CACHE(32B, 0, 32, 1, &___slab_32B)
CACHE(64B, 0, 64, 1, &___slab_64B)
CACHE(128B, 0, 128, 1, &___slab_128B)
CACHE(256B, 0, 256, 1, &___slab_256B)
CACHE(512B, 0, 512, 1, &___slab_512B)
CACHE(1024B, 0, 1024, 1, &___slab_1024B)


CacheChain kmem_cache;

// _rb_buddyBlock* getRB_buddyBlock() {
//     return (_rb_buddyBlock*)cache_RB_buddyBlock._alloc();
// }

linked_container<void*>* getLC_8B() {
    return (linked_container<void*> *)cache_lcPointer._alloc();
}

void freeLC_8B(const void* p) {
    SlabHeader* sp = (SlabHeader*)((uint64_t)p - SHSIZE);
    sp->slab->__free(p);
}

void kmemInit() {
    kmem_cache.insert(&___cacheLC_8B);
    kmem_cache.insert(&___cacheLC_16B);
    kmem_cache.insert(&___cacheLC_32B);
    kmem_cache.insert(&___cacheLC_64B);
    kmem_cache.insert(&___cacheLC_128B);
    kmem_cache.insert(&___cacheLC_256B);
    kmem_cache.insert(&___cacheLC_512B);
    kmem_cache.insert(&___cacheLC_1024B);

    kmem_cache.insert(&___cacheLC_slab);
    kmem_cache.insert(&___cacheLC_lcPointer);
    // kmem_cache.insert(&___cacheLC_RB_buddyBlock);

    buddyInit();
}
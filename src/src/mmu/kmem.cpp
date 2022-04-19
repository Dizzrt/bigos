#include "mmu\kmem.h"
#include "io.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
// linked_container<pointer>
STATIC_SLAB(lcPointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(lcPointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(mSeg, 0, SIZE_mSeg)
STATIC_SLAB(rb_tree_8_8, 0, sizeof(_rb_tree_node<uint64_t, uint64_t>))

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
CACHE(mSeg, 0, SIZE_mSeg, 1, &___slabLC_mSeg)
CACHE(rb_tree_8_8, 0, sizeof(_rb_tree_node<uint64_t, uint64_t>), 1, &___slabLC_rb_tree_8_8)

CACHE(8B, 0, 8, 1, &___slab_8B)
CACHE(16B, 0, 16, 1, &___slab_16B)
CACHE(32B, 0, 32, 1, &___slab_32B)
CACHE(64B, 0, 64, 1, &___slab_64B)
CACHE(128B, 0, 128, 1, &___slab_128B)
CACHE(256B, 0, 256, 1, &___slab_256B)
CACHE(512B, 0, 512, 1, &___slab_512B)
CACHE(1024B, 0, 1024, 1, &___slab_1024B)


CacheChain kmem_cache;

void kmem_free(const void* p) {
    SlabHeader* sp = (SlabHeader*)((uint64_t)p - SHSIZE);
    sp->slab->__free(p);
}

linked_container<void*>* getLC_8B() {
    return (linked_container<void*> *)cache_lcPointer._alloc();
}

void* getMSeg() {
    return cache_mSeg._alloc();
}

void* getRbTree_8_8() {
    return cache_rb_tree_8_8._alloc();
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

    kmem_cache.insert(&___cacheLC_mSeg);
    kmem_cache.insert(&___cacheLC_slab);
    kmem_cache.insert(&___cacheLC_lcPointer);
    kmem_cache.insert(&___cacheLC_rb_tree_8_8);

    buddyInit();
}
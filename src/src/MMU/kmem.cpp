#include "mmu\kmem.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(lcPointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(lcPointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(RB_buddyBlock_0, SLAB_PERMANENT, RB_BUDDY_SIZE)

CACHE(slab, CACHE_NONEMPTY, sizeof(Slab), 2, &___slabLC_slab_0, &___slabLC_slab_1)
CACHE(lcPointer, 0, sizeof(linked_container<void*>), 2, &___slabLC_lcPointer_0, &___slabLC_lcPointer_1)
CACHE(RB_buddyBlock, 0, RB_BUDDY_SIZE, 1, &___slab_RB_buddyBlock_0);

CacheChain kmem_cache;

_rb_buddyBlock* getRB_buddyBlock() {
    return (_rb_buddyBlock*)cache_RB_buddyBlock._alloc();
}

void kmemInit() {
    kmem_cache.insert(&___cacheLC_slab);
    kmem_cache.insert(&___cacheLC_lcPointer);

}
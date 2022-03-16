#include "mmu\kmem.h"
#include "io.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(lcPointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(lcPointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(RB_buddyBlock_0, SLAB_PERMANENT, RB_BUDDY_SIZE)

CACHE(slab, CACHE_NONEMPTY, sizeof(Slab), 2, &___slabLC_slab_0, &___slabLC_slab_1)
CACHE(lcPointer, CACHE_NONEMPTY, sizeof(linked_container<void*>), 2, &___slabLC_lcPointer_0, &___slabLC_lcPointer_1)
CACHE(RB_buddyBlock, 0, RB_BUDDY_SIZE, 1, &___slab_RB_buddyBlock_0);

CacheChain kmem_cache;

_rb_buddyBlock* getRB_buddyBlock() {
    return (_rb_buddyBlock*)cache_RB_buddyBlock._alloc();
}

linked_container<void*>* getLC_8B() {
    return (linked_container<void*> *)cache_lcPointer._alloc();
}

void kmemInit() {
    //buddyInit();

    kmem_cache.insert(&___cacheLC_slab);
    kmem_cache.insert(&___cacheLC_lcPointer);
    kmem_cache.insert(&___cacheLC_RB_buddyBlock);

    printk_svga("slab0:%d\n", ___slab_lcPointer_0.freeObjs);
    printk_svga("slab1:%d\n", ___slab_lcPointer_1.freeObjs);

    for (int i = 0;i < 205;i++) {
        linked_container<void*>* temp = getLC_8B();
    }
    printk_svga("slab0:%d\n", ___slab_lcPointer_0.freeObjs);
    printk_svga("slab1:%d\n", ___slab_lcPointer_1.freeObjs);

}
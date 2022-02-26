#include "mmu\kmem.h"
#include "io.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(lcPointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(lcPointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))

CACHE(slab, CACHE_NONEMPTY, sizeof(Slab), 2, &___slabLC_slab_0, &___slabLC_slab_1)
CACHE(lcPointer, 0, sizeof(linked_container<void*>), 2, &___slabLC_lcPointer_0, &___slabLC_lcPointer_1)

CacheChain kmem_cache;

void kmemInit() {
    kmem_cache.insert(&___cacheLC_slab);
    kmem_cache.insert(&___cacheLC_lcPointer);

    void* test = kmem_cache.alloc(sizeof(long));
    printk_svga("test = 0x%llx\n", test);

    Slab* s = ((SlabHeader*)((uint64_t)test - SHSIZE))->slab;
    void* test2 = s->__alloc();
    printk_svga("test2 = 0x%llx\n", test2);

    printk_svga("freeObjs= %d\n", s->freeObjs);
    s->__free(test);
    test = nullptr;
    printk_svga("test = 0x%llx\n", test);
    printk_svga("freeObjs= %d\n", s->freeObjs);
    test = s->__alloc();
    printk_svga("freeObjs= %d\n", s->freeObjs);
    printk_svga("0x%llx\n", test);
}
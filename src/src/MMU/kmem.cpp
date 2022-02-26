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

    printk_svga("cache_slab.empty.size= %d\n", cache_slab.empty.size());
    printk_svga("cache_lcPointer.empty.size= %d\n", cache_lcPointer.empty.size());

    void* test = kmem_cache.alloc(sizeof(long));
    printk_svga("kmem_cache.alloc = 0x%llx\n", test);

    Slab* s = ((SlabHeader*)((uint64_t)test - SHSIZE))->slab;
    printk_svga("freeObjs= %d\n", s->freeObjs);

    printk_svga("0x%llx\n", s->__alloc());
    printk_svga("freeObjs= %d\n", s->freeObjs);
    printk_svga("cache_slab.empty.size= %d\n", cache_slab.empty.size());
    printk_svga("cache_lcPointer.empty.size= %d\n", cache_lcPointer.empty.size());

    printk_svga("cache_lcPointer.partial.size= %d\n", cache_lcPointer.partial.size());
}
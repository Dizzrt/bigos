#include "mmu\kmem.h"
#include "io.h"


MemPool kmempool;

SLAB_STATIC(1B, sizeof(uint8_t), 0)
SLAB_STATIC(2B, sizeof(uint16_t), 0)
SLAB_STATIC(4B, sizeof(uint32_t), 0)
SLAB_STATIC(8B, sizeof(uint64_t), 0)

SLAB_STATIC(mseg, sizeof(MSeg), 0)
SLAB_STATIC(vmseg, sizeof(VMSeg), 0)
SLAB_STATIC(cache, sizeof(Cache), 0)
SLAB_STATIC(slab_0, sizeof(Slab), 0)
SLAB_STATIC(slab_1, sizeof(Slab), 0)
SLAB_STATIC(lcPoniter, sizeof(linked_container<void*>), 0)

CACHE_STATIC(1B, sizeof(uint8_t), 0, 1, &__slabLC_1B)
CACHE_STATIC(2B, sizeof(uint16_t), 0, 1, &__slabLC_2B)
CACHE_STATIC(4B, sizeof(uint32_t), 0, 1, &__slabLC_4B)
CACHE_STATIC(8B, sizeof(uint64_t), 0, 1, &__slabLC_8B)
CACHE_STATIC(mseg, sizeof(MSeg), 0, 1, &__slabLC_mseg)
CACHE_STATIC(vmseg, sizeof(VMSeg), 0, 1, &__slabLC_vmseg)
CACHE_STATIC(cache, sizeof(Cache), 0, 1, &__slabLC_cache)

CACHE_STATIC(
    lcPointer,
    sizeof(linked_container<void*>), 0,
    1, &__slabLC_lcPoniter
)

CACHE_STATIC(
    slab,
    sizeof(Slab),
    0,
    2, &__slabLC_slab_0, &__slabLC_slab_1
)


ChacheChain kmem_cache;

void slab_free(const void* p) {
    SlabHeader* sh = (SlabHeader*)((uint64_t)p - SHSIZE);
    if (sh->magic == SH_MAGIC)
        sh->slab->free_slab(p);
}


//inner alloc
Slab* kmemAlloc_slab() { return (Slab*)cache_slab.alloc_cache(); }
MSeg* kmemAlloc_mseg() { return (MSeg*)cache_mseg.alloc_cache(); }
void* kmemAlloc_lcPonter() { return cache_lcPointer.alloc_cache(); }
VMSeg* kmemAlloc_vmseg() { return (VMSeg*)cache_vmseg.alloc_cache(); }
Cache* kmemAlloc_Cache() { return (Cache*)cache_cache.alloc_cache(); }
//inner alloc end

void* Kmem_slab_alloc(uint32_t size) { return kmem_cache.alloc(size); }
void* Kmem_page_alloc(uint32_t pages) { return kmempool.alloc(pages); }

void kmem_init() {
    kmem_cache.__insert_cache__(&cacheLC_1B);
    kmem_cache.__insert_cache__(&cacheLC_2B);
    kmem_cache.__insert_cache__(&cacheLC_4B);
    kmem_cache.__insert_cache__(&cacheLC_8B);
    kmem_cache.__insert_cache__(&cacheLC_slab);
    kmem_cache.__insert_cache__(&cacheLC_mseg);
    kmem_cache.__insert_cache__(&cacheLC_vmseg);
    kmem_cache.__insert_cache__(&cacheLC_cache);
    kmem_cache.__insert_cache__(&cacheLC_lcPointer);

    buddy_init();
    kmempool_init();
}

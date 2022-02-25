#include "mmu\kmem.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab))
STATIC_SLAB(linked_container_pointer_0, SLAB_PERMANENT, sizeof(linked_container<void*>))
STATIC_SLAB(linked_container_pointer_1, SLAB_PERMANENT, sizeof(linked_container<void*>))

CACHE(slab, CACHE_NONEMPTY, sizeof(Slab))
CACHE(lcPointer, 0, sizeof(linked_container<void*>))

CacheChain kmem_cache;

void kmemInit() {}
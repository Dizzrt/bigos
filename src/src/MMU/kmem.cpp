#include "mmu\kmem.h"

STATIC_SLAB(slab_0, sizeof(Slab))
STATIC_SLAB(slab_1, sizeof(Slab))
STATIC_SLAB(linked_container_pointer_0, sizeof(linked_container<void*>))
STATIC_SLAB(linked_container_pointer_1, sizeof(linked_container<void*>))

CACHE(slab, sizeof(Slab))
CACHE(lcPointer, sizeof(linked_container<void*>))

void kmemInit() {}
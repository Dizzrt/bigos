#include "mmu\kmem.h"

STATIC_SLAB(slab_0, SLAB_PERMANENT, sizeof(Slab), 1)
STATIC_SLAB(slab_1, SLAB_PERMANENT, sizeof(Slab), 1)

STATIC_CACHE(slab, CACHE_NOEMPTY, sizeof(Slab), SlabSize::x1,
    2, &__slabLC_slab_0, &__slabLC_slab_1);


void kmem_init() {


}
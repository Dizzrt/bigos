#include "mmu\memory.h"
#include "mmu\kmem.h"

void* kmalloc(unsigned int size) {
    if (size < SLAB_OBJSIZE_LIMIT)
        return Kmem_slab_alloc(size);

    size = size / PAGE_SIZE + (size % PAGE_SIZE == 0 ? 0 : 1);
    return Kmem_page_alloc(size);
}

void kfree(const void* p) {
    //TODO kfree

    return;
}
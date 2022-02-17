#include "MMU\memory.h"

MemoryInfo memInfo;
extern int strcmp(const char*, const char*);

void* kmalloc(uint32_t len, uint8_t flags) {
    void* ret = nullptr;

    do {
        if (len >= 0x1000)
            ;  // TODO alloc pages

        ret = kmem_cache.alloc(len);
    } while (ret == nullptr);

    return ret;
}

void kfree(const void* p) {
    SlabHeader* sheader = (SlabHeader*)p - 1;
    if (sheader->magic == slabHeaderMagic)
        sheader->slab->__free(p);
    else
        ;  // TODO free pages
}

// void* alloc_pages(uint32_t pages) { return __buddy_alloc(pages); }
// void free_pages(const void* p) { free_pages(p, alloc_mp[p]); }

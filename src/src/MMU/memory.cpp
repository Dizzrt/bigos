#include "MMU\memory.h"
#include "MMU\buddy.h"
#include "MMU\slab.h"

MemoryInfo memInfo;

void* kmalloc(uint64_t len) {
    if (len > 4096)
        return __buddy_alloc(len / 4096 + (len % 4096 == 0 ? 0 : 1));

    return common_cache.__alloc(len + sizeof(SlabAllocHeader), true);
}

void kfree(const void* p) {
    SlabAllocHeader* sheader = (SlabAllocHeader*)((uint64_t)p - sizeof(SlabAllocHeader));

    // small memory allocated by kmalloc
    if (!strcmp("_SLAB", sheader->magic)) {
        common_cache.__free(sheader->slab, (uint64_t)sheader, sheader->len);
        return;
    }

    if (!alloc_mp.count(p))  // not a valid memory
        return;

    Alloc_Header* header = alloc_mp[p];
    if (header->isPages)
        free_pages(p);
    else
        __slab_free(header);  // small memory, but not allocated by kmalloc
}

void* alloc_pages(unsigned int pages) { return __buddy_alloc(pages); }
void free_pages(const void* p) { free_pages(p, alloc_mp[p]); }
void free_pages(const void* p, Alloc_Header* header) { __buddy_free((uint64_t)p, header->len); }

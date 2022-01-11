#include "MMU\memory.h"
#include "MMU\buddy.h"
#include "MMU\slab.h"

MemoryInfo memInfo;

void* kmalloc(uint64_t len) {
    if (len > 4096)
        return __buddy_alloc(len / 4096 + (len % 4096 == 0 ? 0 : 1));

    void* ret = common_cache.__alloc(len + sizeof(SlabAllocHeader));
    ret = (void*)((uint64_t)ret + sizeof(SlabAllocHeader));
    return ret;
}

void kfree(const void* p) {
    uint64_t addr = (uint64_t)p;

    addr -= sizeof(SlabAllocHeader);
    SlabAllocHeader* header = (SlabAllocHeader*)addr;

    if (!strcmp(header->magic, "_SLAB")) {
        uint64_t _offset = (uint64_t)p - header->slab->vaddr;
        bitmap_update(&header->slab->bitmap, _offset, header->len, false);
        header->slab->objFree += header->len;
    } else
        __buddy_free(const_cast<void*>(p));
}

void* alloc_pages(int pages) { return __buddy_alloc(pages); }
void free_pages(void* p) {}

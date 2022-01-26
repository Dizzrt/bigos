#include "MMU\memory.h"

MemoryInfo memInfo;
extern int strcmp(const char*, const char*);

void* kmalloc(uint32_t len, uint8_t flags) {
    uint32_t xlen = len + sizeof(Common_Cache_Header);

    if (xlen > 4096)
        return alloc_pages(len / 4096 + (len % 4096 == 0 ? 0 : 1));

    return common_cache.alloc_(xlen, flags, true);
}

void kfree(const void* p) {
    uint64_t pp = (uint64_t)p + sizeof(Common_Cache_Header);
    Common_Cache_Header* header = (Common_Cache_Header*)pp;

    if (!strcmp("_SLAB", header->magic))
        header->slab->__free(pp - header->slab->__page, header->len);

    // TODO free pages
}

void* alloc_pages(uint32_t pages) { return __buddy_alloc(pages); }
void free_pages(const void* p) { free_pages(p, alloc_mp[p]); }
void free_pages(const void* p, Alloc_Header* header) { __buddy_free((uint64_t)p, header->len); }

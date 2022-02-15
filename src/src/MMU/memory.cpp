#include "MMU\memory.h"

MemoryInfo memInfo;
extern int strcmp(const char*, const char*);

void* kmalloc(uint32_t len, uint8_t flags) {}

void kfree(const void* p) {}

// void* alloc_pages(uint32_t pages) { return __buddy_alloc(pages); }
// void free_pages(const void* p) { free_pages(p, alloc_mp[p]); }

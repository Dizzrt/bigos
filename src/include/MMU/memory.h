#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "kmem.h"

struct MemoryInfo {
    uint64_t size;
    uint64_t free;
};
extern MemoryInfo memInfo;

void* kmalloc(uint32_t, uint8_t = 0);
void kfree(const void*);

void* alloc_pages(uint32_t = 1);
void free_pages(const void*);
// void free_pages(const void*, Alloc_Header*);

#endif
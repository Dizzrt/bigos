#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "MMU\buddy.h"
#include "MMU\slab.h"
#include "kmem.h"
#include "string.h"

struct MemoryInfo {
    uint64_t size;
    uint64_t free;
};

struct SlabAllocHeader {
    uint16_t len;
    const char magic[6];

    SlabAllocHeader(uint16_t len) : magic("_SLAB") {}
};

extern MemoryInfo memInfo;

void* kmalloc(uint64_t);
void kfree(const void*);

void* alloc_pages(unsigned int = 1);
void free_pages(const void*);
void free_pages(const void*, Alloc_Header*);

#endif
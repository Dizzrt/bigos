#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "bitmap.h"
#include "kmem.h"

struct MemoryInfo {
    uint64_t size;
    uint64_t free;
};

extern MemoryInfo memInfo;

void* kmalloc(uint64_t);
void kfree(const void*);

void* alloc_pages(unsigned int = 1);
void free_pages(const void*);
void free_pages(const void*, Alloc_Header*);

#endif
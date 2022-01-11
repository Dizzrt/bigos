#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "bitmap.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

struct MemoryInfo {
    uint64_t size;
    uint64_t free;
};

extern MemoryInfo memInfo;

void* kmalloc(uint64_t);
void kfree(const void*);

void* alloc_pages(int = 1);
void free_pages(void*);

#endif
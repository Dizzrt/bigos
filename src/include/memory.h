#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "mmu\kmem.h"

void* kmalloc(uint32_t size);
void kfree(const void* p);

#endif
#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "mmu\kmem.h"

void* kmalloc(uint64_t = 1);
void* kfree(const void*);

#endif
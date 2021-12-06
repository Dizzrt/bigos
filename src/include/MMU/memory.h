/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 21:01:36
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

#include "bitmap.h"
#include "slab.h"
#include "stdint.h"

extern Slab iSlab_0; // initial slab
extern Slab iSlab_1;

extern __list_node<Slab *> iSlabNode_0;
extern __list_node<Slab *> iSlabNode_1;

extern Slab_cache kmem_cache;

struct AMS // available memory segment
{
    uint64_t base;
    uint64_t len;
};

struct MemoryPoolNode {
    uint64_t len;
    uint64_t base;
    BitMap bitmap;

    MemoryPoolNode *next = nullptr;
};

static uint64_t totalMsemory;
static MemoryPoolNode *MemoryPools;

void memory_init();
void *__malloc__(size_t);

void *kmalloc(size_t);

#endif
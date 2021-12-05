/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 16:59:38
 * @LastEditTime: 2021-12-05 18:09:24
 * @LastEditors: Please set LastEditors
 * @FilePath: \bigos\src\include\memory.h
 * @Description:
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

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
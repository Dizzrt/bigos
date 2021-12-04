/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 16:59:38
 * @LastEditTime: 2021-12-04 20:53:26
 * @LastEditors: Please set LastEditors
 * @FilePath: \bigos\src\include\memory.h
 * @Description:
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "bitmap.h"
#include "slab.h"
#include "stdint.h"

static Slab permanentSlab;
static Slab_cache kmem_cache;
static __list_node<Slab *> pSlabNode;

struct MemoryPoolNode {
    uint64_t len;
    uint64_t base;
    BitMap bitmap;

    MemoryPoolNode *next = nullptr;
};

static uint64_t totalMsemory;
static MemoryPoolNode *MemoryPools;

void memory_init();
void *__malloc__(size_t len);

// void *kmalloc(size_t len) {}

#endif
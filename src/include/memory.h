/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 16:59:38
 * @LastEditTime: 2021-11-29 13:44:12
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\include\memory.h
 * @Description:
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "bitmap.h"
#include "stdint.h"

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

#endif
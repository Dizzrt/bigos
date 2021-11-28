/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 17:05:29
 * @LastEditTime: 2021-11-28 22:06:07
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\memory.cpp
 * @Description:
 */

#include "memory.h"
#include "io.h"

void init_memory() {
    uint32_t amsCount = *((uint32_t *)0x504);
    uint64_t *ams = (uint64_t *)0x508; // available memory segment

    // the first bitmap
    MemoryPools = (MemoryPoolNode *)0x400002b000;
    MemoryPools->bitmap.bits = (uint8_t *)0x400002b100;
    MemoryPools->base = *ams++;
    MemoryPools->len = *ams++;                         // how much bytes in this pool
    MemoryPools->bitmap.len = MemoryPools->len / 4096; // how much pages in this bitmap

    *MemoryPools->bitmap.bits = 0xff;
    *(MemoryPools->bitmap.bits + 1) = 0x80;

    for (int i = 1; i < amsCount; i++) {}

    return;
}

void *__malloc__(size_t len) {
    void *addr;
    len = len / 4096 + (len % 4096 == 0 ? 0 : 1);
    // TODO malloc

    uint64_t offset = -1;
    MemoryPoolNode *mpool = MemoryPools;
    while (true) {
        offset = bitmap_scan(mpool->bitmap, len);
        if (offset != -1)
            break;
        else if (mpool->next != nullptr)
            mpool = mpool->next;
    }

    if (offset == -1)
        addr = nullptr;
    else
        addr = (void *)(offset * 0x1000 + mpool->base);

    // TODO update bitmap

    return addr;
}
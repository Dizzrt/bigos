/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 17:05:29
 * @LastEditTime: 2021-11-29 16:19:28
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\memory.cpp
 * @Description:
 */

#include "memory.h"
#include "io.h"

void memory_init() {
    uint32_t amsCount = *((uint32_t *)0x504);
    uint64_t *ams = (uint64_t *)0x508; // available memory segment

    // the first bitmap
    MemoryPools = (MemoryPoolNode *)0x400002b000;
    MemoryPools->bitmap.bits = (uint8_t *)0x400002b100;
    MemoryPools->base = *ams++;
    MemoryPools->len = *ams++;                         // how much bytes in this pool
    MemoryPools->bitmap.len = MemoryPools->len / 4096; // how much pages in this bitmap

    bitmap_init(&MemoryPools->bitmap); // set all bits to 1(available page)

    // top 10 pages (0x0~0x9000) are used
    *MemoryPools->bitmap.bits = 0;
    *(MemoryPools->bitmap.bits + 1) = 0x3f;

    // MemoryPoolNode *mnode = MemoryPools->next;
    // for (int i = 1; i < amsCount; i++) {
    //     mnode = (MemoryPoolNode *)__malloc__(sizeof(MemoryPoolNode));
    //     mnode->base = *ams++;
    //     mnode->len = *ams++;
    //     mnode->bitmap.len = mnode->len / 4096;
    //     mnode->bitmap.bits = (uint8_t *)__malloc__(mnode->bitmap.len / 8 + (mnode->bitmap.len % 8 ? 1 : 0));
    //     bitmap_init(&mnode->bitmap);

    //     mnode = mnode->next;
    // }

    return;
}

void *__malloc__(size_t len) {

    len = len / 4096 + (len % 4096 ? 1 : 0);
    // TODO malloc

    //按页分配
    uint64_t offset = -1;
    MemoryPoolNode *mpool = MemoryPools;
    while (true) {
        offset = bitmap_scan(&mpool->bitmap, len);
        if (offset != -1)
            break;
        else if (mpool->next != nullptr)
            mpool = mpool->next;
        else
            return nullptr;
    }

    void *addr = (void *)(offset * 0x1000 + mpool->base);

    // TODO update bitmap

    return addr;
}
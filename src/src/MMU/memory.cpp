/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-11-30 15:49:35
 */

#include "MMU\memory.h"

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

    // top 44 pages (0x0~0x9000) are used
    bitmap_update(&MemoryPools->bitmap, 0, 44);

    MemoryPoolNode *curPool = MemoryPools;
    for (int i = 1; i < amsCount; i++) {
        curPool->next = (MemoryPoolNode *)__malloc__(sizeof(MemoryPoolNode));
        curPool = curPool->next;
        curPool->base = *ams++;
        curPool->len = *ams++;
        curPool->bitmap.len = curPool->len / 4096;
        curPool->bitmap.bits = (uint8_t *)__malloc__(curPool->bitmap.len / 8 + (curPool->bitmap.len % 8 ? 1 : 0));
        bitmap_init(&curPool->bitmap);
    }

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

    uint64_t addr = offset * 0x1000 + mpool->base; // this is a physical address

    // TODO convert pd to vd
    // TODO make page table
    { // temporary,just for kernel
        addr += 0x4000000000;
    }

    bitmap_update(&mpool->bitmap, offset, len);

    return (void *)addr;
}
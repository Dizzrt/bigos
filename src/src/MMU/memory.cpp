/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-12 17:44:11
 */

#include "MMU\memory.h"
#include "MMU\buddy.h"

Slab iSlab_0;
Slab iSlab_1;

__list_node<Slab *> iSlab_lnode_0;
__list_node<Slab *> iSlab_lnode_1;

MemoryInfo memInfo;
Slab_cache common_cache;

void memory_init() {
    //----common cache init----
    // 0x600-0x9ff are used as bitmap of slab
    iSlab_0.flags = SLAB_PERMANENT;
    iSlab_0.vaddr = 0x400001000;
    iSlab_0.bitmap.bits = (uint8_t *)0x600;
    bitmap_init(&iSlab_0.bitmap);

    iSlab_1.flags = SLAB_PERMANENT;
    iSlab_1.vaddr = 0x4000002000;
    iSlab_1.bitmap.bits = (uint8_t *)0x800;
    bitmap_init(&iSlab_1.bitmap);

    iSlab_lnode_0.val = &iSlab_0;
    iSlab_lnode_1.val = &iSlab_1;

    common_cache.slabs_available.__list_add(&iSlab_lnode_0);
    common_cache.slabs_available.__list_add(&iSlab_lnode_1);
    //----end common cache init----

    //----buddy sys init----
    // int amsCount = *((int *)0x504);
    // AMS *ams = (AMS *)0x508; // available memory segment

    // int blockCnt[11] = {0};
    // int blockSize[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    // for (int i = 0; i < amsCount; i++) {
    //     memInfo.size += ams[i].len;
    //     uint64_t paddr = ams[i].base;
    //     uint64_t page_cnt = ams[i].len / 4096;

    //     for (int j = 10; j >= 0; j--) {
    //         blockCnt[j] = page_cnt / blockSize[j];
    //         page_cnt %= blockCnt[j];
    //         if (!page_cnt)
    //             break;
    //     }

    //     for (int j = 0; j < 11; j++) {
    //         while (blockCnt[j]--) {
    //             Block *tblock = (Block *)kmalloc(sizeof(Block));
    //             tblock->addr = paddr;
    //             paddr += blockSize[j] * 4096;
    //             blist[j].push_back(tblock);
    //         }
    //     }
    // }
    // end buddy sys init----

    return;
}

void *kmalloc(uint64_t len) {
    if (len < 4096)
        return common_cache.__alloc(len);
    else {
        // TODO get page from buddy
        return nullptr;
    }
}

/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-09 17:44:49
 */

#include "MMU\memory.h"

Slab iSlab_0;
Slab iSlab_1;

__list_node<Slab *> iSlabNode_0;
__list_node<Slab *> iSlabNode_1;

Slab_cache kmem_cache;

void memory_init() {
    // kmem_cache.__tmporary_init(); // MARKER temp

    //----slab init----
    // 0x600-0x9ff are used as bitmap of slab
    iSlab_0.__using = 0;
    iSlab_0.__free = 0x1000;
    iSlab_0.bitmap.len = 0x1000;
    iSlab_0.bitmap.bits = (uint8_t *)0x600;

    bitmap_init(&iSlab_0.bitmap);
    iSlab_0.vaddr = 0x4000001000;

    iSlab_1.__using = 0;
    iSlab_1.__free = 0x1000;
    iSlab_1.bitmap.len = 0x1000;
    iSlab_1.bitmap.bits = (uint8_t *)0x800;

    bitmap_init(&iSlab_1.bitmap);
    iSlab_1.vaddr = 0x4000002000;

    iSlab_0.type = SlabType::PERMANENT;
    iSlab_1.type = SlabType::PERMANENT;

    iSlabNode_0.val = &iSlab_0;
    iSlabNode_1.val = &iSlab_1;

    kmem_cache.__appendSlab_(&iSlabNode_0);
    kmem_cache.__appendSlab_(&iSlabNode_1);
    //----end----

    // uint32_t amsCount = *((uint32_t *)0x504);
    // AMS *ams = (AMS *)0x508; // available memory segment

    // // the first bitmap
    // MemoryPools = (MemoryPoolNode *)0x400002b000;
    // MemoryPools->bitmap.bits = (uint8_t *)0x400002b100;
    // MemoryPools->base = ams->base;
    // MemoryPools->len = ams->len;                       // how much bytes in this pool
    // MemoryPools->bitmap.len = MemoryPools->len / 4096; // how much pages in this bitmap

    // bitmap_init(&MemoryPools->bitmap); // set all bits to 1(available page)

    // // top 44 pages (0x0~0x9000) are used
    // bitmap_update(&MemoryPools->bitmap, 0, 44);

    // ams++;
    // MemoryPoolNode *curPool = MemoryPools;
    // for (int i = 1; i < amsCount; i++) {
    //     curPool->next = (MemoryPoolNode *)__malloc__(sizeof(MemoryPoolNode));
    //     curPool = curPool->next;
    //     curPool->base = ams->base;
    //     curPool->len = ams->len;
    //     curPool->bitmap.len = curPool->len / 4096;
    //     curPool->bitmap.bits = (uint8_t *)__malloc__(curPool->bitmap.len / 8 + (curPool->bitmap.len % 8 ? 1 : 0));
    //     bitmap_init(&curPool->bitmap);
    // }

    return;
}

void *kmalloc(size_t len) {
    if (len < 4096)
        return kmem_cache.__alloc(len);
    else {
        // TODO get page from buddy
    }
}

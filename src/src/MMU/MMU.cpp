#include "MMU\MMU.h"
#include "MMU\buddy.h"
#include "MMU\memory.h"
#include "MMU\slab.h"

extern uint64_t ENDTAG;  // how big is the kernel

void memory_init() {
    //----common cache init----
    // 0x600-0x9ff are used as bitmap of slab
    iSlab_0.flags = SLAB_PERMANENT;
    iSlab_0.vaddr = 0xffff800000001000;
    iSlab_0.bitmap.bits = (uint8_t*)0xffff800000000800;
    bitmap_init(&iSlab_0.bitmap);

    iSlab_1.flags = SLAB_PERMANENT;
    iSlab_1.vaddr = 0xffff800000002000;
    iSlab_1.bitmap.bits = (uint8_t*)0xffff800000000a00;
    bitmap_init(&iSlab_1.bitmap);

    iSlab_lnode_0.val = &iSlab_0;
    iSlab_lnode_1.val = &iSlab_1;

    common_cache.objSize = 1;
    common_cache.slabs_available.__list_add(&iSlab_lnode_0, common_cache.slabs_available.end());
    common_cache.slabs_available.__list_add(&iSlab_lnode_1, common_cache.slabs_available.end());
    //----end common cache init----

    //----buddy sys init----
    buddy_slab_objCnt = 4096 / sizeof(__buddy_node);

    buddy_slab.flags = SLAB_PERMANENT;
    buddy_slab.vaddr = 0xffff80000000f000;
    buddy_slab.bitmap.bits = (uint8_t*)kmalloc(buddy_slab_objCnt / 8 + (buddy_slab_objCnt % 8 == 0 ? 0 : 1));

    buddy_slab.objFree = buddy_slab_objCnt;
    buddy_slab.bitmap.len = buddy_slab_objCnt;
    bitmap_init(&buddy_slab.bitmap);

    iBuddy_Slab_lnode.val = &buddy_slab;
    buddy_node_cache.objSize = sizeof(__buddy_node);
    buddy_node_cache.slabs_available.__list_add(&iBuddy_Slab_lnode, buddy_node_cache.slabs_available.end());

    __buddy_free(0x40000, 64);

    uint64_t valid_t = ((ENDTAG - 0xffff800000000000) / 4096 + 1) * 4096;
    int amsCount = *((int*)0xffff800000000504);
    AMS* ams = (AMS*)0xffff800000000508;

    int bsize[] = {4096, 2 * 4096, 4 * 4096, 8 * 4096, 16 * 4096, 32 * 4096, 64 * 4096, 128 * 4096, 256 * 4096,
        512 * 4096, 1024 * 4096};

    for (int i = 1; i < amsCount; i++) {  // the first segment is in the lower 1MB space,so skip
        if (ams[i].base + ams[i].len < valid_t)
            continue;

        if (ams[i].base < valid_t) {
            uint64_t temp = valid_t - ams[i].base;
            ams[i].base = valid_t;
            ams[i].len -= temp;
        }

        if (ams[i].base % 4096) {  // 4K alignment
            uint64_t temp = 4096 - ams[i].base % 4096;
            ams[i].base += temp;
            ams[i].len -= temp;
        }

        for (int j = 10; j >= 0; j--) {
            while (ams[i].len >= bsize[j]) {
                ams[i].len -= bsize[j];
                __buddy_free(ams[i].base, __buddy_lens[j]);
                ams[i].base += bsize[j] * 0x1000;
            }
        }
    }
    // end buddy sys init----

    return;
}
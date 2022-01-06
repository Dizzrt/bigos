/*
 * @Author: Dizzrt
 * @LastEditTime: 2022-01-02 21:05:12
 */

#include "MMU\memory.h"
#include "MMU\buddy.h"
#include "MMU\slab.h"
#include "io.h"

Slab iSlab_0;
Slab iSlab_1;

__list_node<Slab*> iSlab_lnode_0;
__list_node<Slab*> iSlab_lnode_1;

MemoryInfo memInfo;
Slab_cache common_cache;

extern uint64_t ENDTAG;

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

    create_buddy_node(0, 0x40000, BUDDY_NODE_LEN::B2);
    create_buddy_node(0, 0x42000, BUDDY_NODE_LEN::B2);
    create_buddy_node(0, 0x44000, BUDDY_NODE_LEN::B4);
    create_buddy_node(0, 0x48000, BUDDY_NODE_LEN::B8);
    create_buddy_node(0, 0x50000, BUDDY_NODE_LEN::B16);
    create_buddy_node(0, 0x60000, BUDDY_NODE_LEN::B32);

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
                create_buddy_node(0, ams[i].base, (BUDDY_NODE_LEN)j);
                ams[i].base += bsize[j] * 0x1000;
            }
        }
    }

    buddy_deg();
    void* test = get_one_page();
    printk_svga("\nget one page from buddy:\n");
    printk_svga("physical address: 0x%llx\n", (uint64_t)test);
    printk_svga("virtual address: 0x%llx\n\n", (uint64_t)test + 0xffff800000000000);
    buddy_deg();
    // end buddy sys init----

    return;
}

void* kmalloc(uint64_t len) {
    if (len < 4096)
        return common_cache.__alloc(len);
    else {
        // TODO get page from buddy
        return nullptr;
    }
}

void kfree(const void* p) {
    list<Slab*>::iterator iter = common_cache.slabs_full.begin();

    // TODO
}

void* get_one_page() {
    int lens[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    int lidx = 0;
    while (!blist[lidx].size())
        lidx++;

    while (lidx > 0) {
        __buddy_node* _bnode = *blist[lidx].begin();
        uint64_t midAddr = (lens[lidx] >> 1) * 0x1000 + _bnode->paddr;

        blist[lidx--].pop_front();
        create_buddy_node(0, _bnode->paddr, (BUDDY_NODE_LEN)lidx);
        create_buddy_node(0, midAddr, (BUDDY_NODE_LEN)lidx);
    }

    void* ret = (void*)(*blist[0].begin())->paddr;
    blist[0].pop_front();
    return ret;
}

void* get_pages(int cnt) {
    return nullptr;
    // TODO get pages
}
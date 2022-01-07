#include "MMU\buddy.h"
#include "io.h"

int buddy_slab_objCnt;
Slab buddy_slab;
__list_node<Slab*> iBuddy_Slab_lnode;
Slab_cache buddy_node_cache;

list<__buddy_node*> blist[11];

void create_buddy_node(uint8_t _flags, uint64_t _paddr, BUDDY_NODE_LEN len) {
    __buddy_node* temp = (__buddy_node*)buddy_node_cache.__alloc(1);
    temp->flags = _flags;
    temp->paddr = _paddr;

    uint64_t _endPaddr = _paddr + len * 4096;
    // TODO merge continuous memory
    blist[len].push_back(temp);
}

void buddy_deg() {
    printk_svga("Block Size  :1\t2\t4\t8\t16\t32\t64\t128\t256\t512\t1024\n");
    printk_svga("Block Count :");
    for (int i = 0; i < 11; i++)
        printk_svga("%d\t", blist[i].size());
    putk_svag('\n');
}
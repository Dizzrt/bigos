#include "MMU\buddy.h"
#include "io.h"

int buddy_slab_objCnt;
Slab buddy_slab;
linked_container<Slab*> iBuddy_Slab_lnode;
Slab_cache buddy_node_cache;

list<__buddy_node*> blist[11];

// void buddy_deg() {
//     printk_svga("Block Size  :1\t2\t4\t8\t16\t32\t64\t128\t256\t512\t1024\n");
//     printk_svga("Block Count :");
//     for (int i = 0; i < 11; i++)
//         printk_svga("%d\t", blist[i].size());
//     putk_svag('\n');
// }

void* __buddy_alloc(int pages) {
    if (pages <= 0)
        return nullptr;

    int idx = 0;

    while (pages > __buddy_lens[idx])
        idx++;
    int remain = __buddy_lens[idx] - pages;

    __buddy_node* _bnode = *blist[idx].begin();
    blist[idx].pop_front();

    void* ret = (void*)_bnode->paddr;
    _bnode->paddr += pages * 0x1000;
    __buddy_map[ret] = pages;

    for (int i = 10; i >= 0; i--) {
        while (remain >= __buddy_lens[i]) {
            __buddy_node* temp = __get_buddy_node();
            temp->paddr = _bnode->paddr;

            _bnode->paddr += __buddy_lens[i] * 0x1000;
            blist[i].push_back(temp);
            remain -= __buddy_lens[i];
        }
    }

    return ret;
}

void __buddy_free(uint64_t vaddr, uint32_t len) {
    if (len == 0)
        return;

    for (int i = 10; i >= 0; i--) {
        while (len >= __buddy_lens[i]) {
            __buddy_node* _bnode = __get_buddy_node();
            _bnode->paddr = vaddr;
            blist[i].push_back(_bnode);  // FIXME merge blocks

            len -= __buddy_lens[i];
        }
    }
}

void __buddy_free(void* p) {
    if (!__buddy_map.count(p))
        return;

        __buddy_free((uint64_t)p, __buddy_map[p]);
    __buddy_map.erase(p);
}

__buddy_node* __get_buddy_node() { return (__buddy_node*)buddy_node_cache.__alloc(1); }
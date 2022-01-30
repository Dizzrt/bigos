#include "MMU\buddy.h"

extern Cache buddy_cache;
extern Cache buddyMapNode_cache;

klist<__buddy_node> blist[11];
static _rb_tree<void*, uint32_t> __buddy_map;

void* __buddy_alloc(uint32_t pages) {
    // if (pages <= 0)
    //     return nullptr;

    int idx = 0;

    while (pages > __buddy_lens[idx])
        idx++;
    int remain = __buddy_lens[idx] - pages;

    __buddy_node _bnode = *blist[idx].begin();
    __buddy_node tmp = blist[idx].__list_remove(blist[idx].begin());  // TODO free buddy node

    void* ret = (void*)_bnode.paddr;
    _bnode.paddr += pages * 0x1000;

    buddyMapNode* mNode = (buddyMapNode*)buddyMapNode_cache.alloc_(1);
    new (mNode) buddyMapNode(ret, pages);
    __buddy_map.__insert(mNode);

    for (int i = 10; i >= 0; i--) {
        while (remain >= __buddy_lens[i]) {
            buddy_container* temp = __alloc_buddy_container();
            temp->val.paddr = _bnode.paddr;

            _bnode.paddr += __buddy_lens[i] * 0x1000;
            blist[i].__list_insert(temp, blist[i].end());
            remain -= __buddy_lens[i];
        }
    }

    return ret;
}

void __buddy_free(uint64_t paddr, uint32_t len) {
    if (len == 0)
        return;

    for (int i = 10; i >= 0; i--) {
        while (len >= __buddy_lens[i]) {
            buddy_container* _bnode = __alloc_buddy_container();
            _bnode->val.paddr = paddr;
            blist[i].__list_insert(_bnode, blist[i].end());  // FIXME merge blocks

            len -= __buddy_lens[i];
        }
    }
}

void __buddy_free(void* p) {
    if (!__buddy_map.count(p))
        return;  // not a valid pointer

    __buddy_free((uint64_t)p, __buddy_map.find(p));
    //__buddy_map.__remove(p); //MARKER map remove
}

buddy_container* __alloc_buddy_container() { return (buddy_container*)buddy_cache.alloc_(1); }
void __free_buddy_container(buddy_container* _node) {
    // MARKER optimize

    // Slab* _slab;  // TODO free buddy container

    // list<Slab*>::iterator iter = buddy_cache.
}
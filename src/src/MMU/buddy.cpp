#include "mmu\buddy.h"

static klist<uint64_t> buddyBlocks[11];
static _rb_tree<void*, buddyBlock> buddy_mp;

static uint16_t bls[] = { 1,2,4,8,16,32,64,128,256,512,1024 };

void* buddy_alloc(unsigned int pCount) {
    if (pCount > 1024)
        return nullptr;

    int t = 0;
    while (pCount > bls[t])
        t++;

    int rlen = bls[t] - pCount;
    linked_container_iterator<uint64_t> iter = buddyBlocks[t].begin();
    buddyBlocks[t].__list_remove(iter);

    uint64_t np = *iter + 0x1000 * pCount;
    for (int i = 10;rlen > 0 && i >= 0;i++) {
        while (rlen >= bls[i])
        {
            //TODO alloc
            //buddyBlocks[i].__list_insert()
            np += 0x1000 * bls[i];
        }
    }

    np = *iter;
    //TODO free iter.m_node
    return (void*)np;
}

void buddy_free(const void* p) {}


void buddyInit() {
    struct AMS // available memory segment
    {
        uint64_t base;
        uint64_t len;
    };

    AMS* _ams = (AMS*)0xffff800000000508;
    int _amsCount = *((int*)0xffff800000000504);

    for (int i = 1;i < _amsCount;i++) {

    }
}
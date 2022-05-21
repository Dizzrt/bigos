#include "mmu\mempool.h"
#include "new.h"

VirtualSeg* VirtualPool::vseg_alloc(uint32_t len) {
    VirtualSeg* ret = nullptr;

    for (auto seg : segs)
        if (seg->len >= len) {
            //TODO alloc mem for ret
            ret->base = seg->base;
            ret->len = len;

            if (seg->len > len) {
                seg->base += 0x1000 * len;
                seg->len -= len;
            }
        }

    return ret;
}

void VirtualPool::vseg_free(VirtualSeg* seg) {
    linked_container<VirtualSeg*>* lseg;//TODO alloc lseg
    new(lseg) linked_container<VirtualSeg*>(seg);

    if (segs.empty()) {
        segs.__list_insert(lseg);
        return;
    }

    klist<VirtualSeg*>::iterator iter = segs.begin();
    klist<VirtualSeg*>::iterator iend = segs.end();

    while (iter != iend)
    {
        if ((*iter)->base > seg->base)
        {

        }
    }


}
#include "mmu\mempool.h"
#include "mmu\buddy.h"
#include "new.h"

void* MemPool::alloc(uint32_t gfp_flags, uint32_t pages) {
    void* ret = nullptr;
    //TODO alloc ret

    if (pages > maxPages_mapped) {
        // TODO alloc by buddy
    }
    else {
        // alloc by mapped_VMSeg

        VMSeg_iter iter = mapped_VMSeg.begin();

        VMSeg* seg = nullptr;
        while (iter != mapped_VMSeg.end())
        {
            if ((*iter)->pages >= pages) {
                seg = *iter;
                break;
            }
            else iter++;
        }

        ret = (void*)seg->base; // alloc address

        if (seg->pages == pages) {
            mapped_VMSeg.__list_remove(iter);

            if (used_VMSeg.empty())
                used_VMSeg.__list_insert(iter);
            else {
                for (VMSeg_iter i = used_VMSeg.begin();i != used_VMSeg.end();i++) {
                    if ((*i)->base > seg->base) {
                        used_VMSeg.__list_insert(iter, i);
                        break;
                    }
                }
            }
        }
        else {
            //TODO divide vmseg
        }
    }

    return ret;
}

void* MemPool::alloc(uint32_t pages) {
    //TODO default flags
    return alloc(0, pages);
}
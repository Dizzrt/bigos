#ifndef __BIG_MEMPOOL_H__
#define __BIG_MEMPOOL_H__

#include "memdef.h"
#include "ktl\klist.h"
// #include "ktl\rb_tree.h"

class MemPool
{
private:
    typedef klist<VMSeg*>::iterator VMSeg_iter;
public:
    klist<VMSeg*> unmapped_VMSeg; // unmapped
    klist<VMSeg*> mapped_VMSeg; // mapped but not used
    klist<VMSeg*> used_VMSeg; // mapped and used

    uint64_t maxPages_mapped;

    void* alloc(uint32_t pages = 1, uint32_t gfp_flags = 0);

    MemPool(/* args */) = default;
    ~MemPool() = default;
};

void kmempool_init();


#endif //__BIG_MEMPOOL_H__
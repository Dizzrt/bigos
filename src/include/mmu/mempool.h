#ifndef __BIG_MEMPOOL_H__
#define __BIG_MEMPOOL_H__

#include "memdef.h"
#include "ktl\klist.h"
#include "ktl\rb_tree.h"

// struct MSeg
// {
//     uint32_t flags;
//     uint32_t segCnt;

//     klist<Pfs*> segs;

//     Pfs* allocSeg();
//     void freeSeg(Pfs*);
// };

struct VirtualSeg // virtual memory segment
{
    uint64_t base;
    uint64_t len;
};

struct VirtualPool
{
    klist<VirtualSeg*> segs;

    //vseg_free(uint64 base,uint64 len)
    void vseg_free(uint64_t, uint64_t);
    void vseg_alloc(uint32_t len);
};

struct AvailablePool
{
    klist<VPfs*> segs_free;
    _rb_tree<void*, VPfs*> segs_using;

    void* aseg_alloc(uint32_t);
};

struct MemPool
{
    VirtualPool vpool;
    AvailablePool apool;

    void* mpool_alloc(uint32_t);
    void mpool_free(void*);
};




#endif //__BIG_MEMPOOL_H__
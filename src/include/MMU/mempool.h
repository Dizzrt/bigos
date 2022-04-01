#ifndef __BIG_MEMPOOL_H__
#define __BIG_MEMPOOL_H__

// #include "ktl\klist.h"
#include "stdint.h"
#include "ktl\rb_tree.h"

extern void* getMSeg();
extern void* getRbTree_8_8();

extern void kmem_free(const void*);

struct mSeg
{
    uint8_t flags;
    uint8_t reserved[7]; //alignment

    uint64_t base;
    uint64_t len;

    mSeg(uint64_t, uint64_t, uint8_t);
};

class MemPool {
private:
    //memory segment node
    typedef _rb_tree_node<uint64_t, mSeg*> mSegNode;

    _rb_tree<uint64_t, mSeg*> freeSegs; // free memory segment
    _rb_tree<uint64_t, mSeg*> usedSegs;

public:
    ~MemPool() = default;

    //alloc pages
    void* _alloc(uint64_t);
    void _free(const void*);

    //base,len,flags
    void _createSeg(uint64_t, uint64_t, uint8_t);
    void _freeSeg(const void*);
};



#endif //__BIG_MEMPOOL_H__
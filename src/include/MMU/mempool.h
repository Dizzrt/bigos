#ifndef __BIG_MEMPOOL_H__
#define __BIG_MEMPOOL_H__

#include "ktl\klist.h"
#include "ktl\kbitset.h"
#include "ktl\rb_tree.h"

class MemPool : public kbitset {
private:
    uint64_t nextBase;
    uint64_t baseLimit;
public:
    ~MemPool() = default;

    void* __alloc(uint64_t);
};


struct MemPoolChain
{
    klist<MemPool*> pools;


    void extend(uint64_t);
    void* _alloc(uint64_t);
};



#endif //__BIG_MEMPOOL_H__
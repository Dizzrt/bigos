#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "mmu\memdef.h"
#include "ktl\kbitset.h"
#include "ktl\klist.h"

class Slab : public kbitset
{
private:
    uint64_t base;
    uint32_t offsetStep;
public:
    uint32_t flags;
    const uint64_t& freeObjs = _fc;

    void* alloc_slab();

    //offset
    void free_slab(uint64_t);

    void free_slab(const void*);

    Slab(/* args */) = default;
    Slab(uint32_t = 0, uint32_t = 1, uint64_t = 0, uint8_t* = nullptr);


    ~Slab() = default;
};

struct SlabHeader
{
    Slab* slab;
    const uint64_t magic; //
};
#define SHSIZE sizeof(SlabHeader)

class Cache
{
    uint32_t flags;
    uint32_t objSize;

    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partial;
public:
    Cache(/* args */) = default;
    ~Cache() = default;

    void* alloc_Cache();
};


class CacheChain
{
private:
    klist<Cache*> cache_list;
public:
    CacheChain(/* args */) = default;
    ~CacheChain() = default;

    void* alloc(uint64_t);

    void insert(Cache*);
    void insert(linked_container<Cache*>*);

    //void CreateCache();
};


#endif //__BIG_SLAB_H__
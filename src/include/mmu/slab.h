#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "mmu\memdef.h"
#include "ktl\kbitset.h"
#include "ktl\klist.h"

#define LONG_ALIGN(SIZE) \
  ((SIZE + sizeof(long) - 1) & ~(sizeof(long) - 1))

class Slab : public kbitset
{
private:
    uint64_t base;
    uint64_t offsetStep;
public:
    uint32_t flags;
    const uint64_t& freeObjs = _fc;

    void* alloc_slab();

    //offset
    void free_slab(uint64_t);

    void free_slab(const void*);

    Slab(/* args */) = default;
    Slab(uint32_t, uint32_t, uint64_t, uint64_t, uint8_t*);


    ~Slab() = default;
};

struct SlabHeader
{
    Slab* slab;
    const uint64_t magic;

    SlabHeader(Slab*);
};
#define SHSIZE sizeof(SlabHeader)
#define SH_magic 123

enum SLABSIZE {
    x1 = 1 * 0x1000, x2 = 2 * 0x1000, x4 = 4 * 0x1000,
    x8 = 8 * 0x1000, x16 = 16 * 0x1000, x32 = 32 * 0x1000,
    x64 = 64 * 0x1000, x128 = 128 * 0x1000, x256 = 256 * 0x1000,
    x512 = 512 * 0x1000, x1024 = 1024 * 0x1000
};

class Cache
{
private:
    uint32_t flags;
    uint32_t _alignment_;

    uint32_t objSize;
    // uint32_t slabSize;
    SLABSIZE slabSize;


    uint32_t ops; //objs per slab
    uint32_t offsetStep;

    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partial;
public:
    Cache(/* args */) = default;
    ~Cache() = default;

    Cache(uint32_t, uint32_t, uint32_t);

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
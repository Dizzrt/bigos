#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "ktl\bitset.h"
#include "ktl\klist.h"
#include "stdint.h"

#define SLAB_PERMANENT 0b10000000

// get a new slab by buddy system instead of cache_slab
//#define CACHE_AUTONOMY 0b10000000

#define LONG_ALIGN(SIZE) ((SIZE + sizeof(long) - 1) & ~(sizeof(long) - 1))

extern uint64_t SH_magic;

class Slab : public bitset {
  private:
    uint64_t page;
    uint16_t offsetSize;

  public:
    uint8_t flags;
    const uint64_t& freeObjs = _fc;

    void* __alloc();

    Slab(uint8_t = 0, uint16_t = 1, uint64_t = -1, uint8_t* = nullptr);
    ~Slab() = default;
};

struct SlabHeader {
    Slab* slab;
    const uint64_t magic;

    SlabHeader(Slab* _slab) : slab(_slab), magic(SH_magic) {}
};
#define SHSIZE sizeof(SlabHeader)

class Cache {
  public:
    // uint8_t flags;
    uint16_t objSize;

    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partial;

    void* _alloc();

    Cache(uint16_t);
    ~Cache() = default;
};

class CacheChain {
  private:
    klist<Cache*> _caList;

  public:
    void insert(linked_container<Cache*>*);
    void* alloc(uint64_t);

    CacheChain(/* args */) = default;
    ~CacheChain() = default;
};

#endif
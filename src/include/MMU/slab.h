#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "ktl\bitset.h"
#include "ktl\klist.h"

uint64_t SH_magic;

struct SlabHeader {
    Slab* slab;
    const uint64_t magic;

    SlabHeader(Slab* _slab) : slab(_slab), magic(SH_magic) {}
};
#define SHSIZE sizeof(SlabHeader)

class Slab : public bitset {
  private:
    uint64_t page;
    uint16_t offsetSize;

  public:
    uint8_t flags;
    const uint64_t& freeObjs = _fc;

    void* __alloc();

    Slab(/* args */) = default;
    ~Slab() = default;
};

class Cache {
  public:
    uint16_t objSize;

    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partial;

    void* _alloc();

    Cache(/* args */) = default;
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
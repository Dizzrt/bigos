#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "KTL\bitset.h"
#include "KTL\klist.h"
#include "new.h"
#include "stdint.h"

#define SLAB_PERMANENT 0b10000000

extern uint64_t slabHeaderMagic;

class Slab : protected bitset {
  public:
    uint8_t __flags;
    uint64_t __page;

    uint16_t freeCnt;
    uint16_t offsetSize;

    Slab(uint8_t = 0, uint16_t = 1, uint64_t = -1, uint8_t* = nullptr);

    // obj_size
    // Slab(uint16_t, uint8_t, )
    //~Slab();

    uint64_t __alloc();
    void __free(const void*);
};

struct SlabHeader {
    Slab* slab;
    const uint64_t magic;

    // TODO magic
    SlabHeader(Slab* _slab = nullptr) : slab(_slab), magic(slabHeaderMagic) {}
};
#define SLABHEADERSIZE sizeof(SlabHeader)

struct Cache {
    uint16_t _objSize;

    klist<Slab> full;
    klist<Slab> empty;
    klist<Slab> partial;

    void* _alloc();

    Cache() = default;
    Cache(uint16_t __objSize) : _objSize(__objSize) {}
};

struct CacheChain {
    klist<Cache*> _caList;

    // Cache& GetCacheBySize(uint64_t _size) { return GetCacheByObjSize(_size - sizeof(SlabHeader)); }
    // MS=>the slab's objSize and target objSize must be same
    // Cache& GetCacheByObjSize(uint64_t _objSize, bool MS = false);

    void* alloc(uint16_t);
};

typedef linked_container<Slab> slab_container;
// slab_container* __alloc_slab_container(uint8_t, uint16_t);
// void __free_slab_container(slab_container*);

#endif
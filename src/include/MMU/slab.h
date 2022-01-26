#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "KTL\bitset.h"
#include "KTL\klist.h"
#include "new.h"
#include "stdint.h"

#define SLAB_PERMANENT 0b10000000

class Slab : protected bitset {
  public:
    uint8_t __flags;
    uint64_t __page;
    uint16_t __free_obj_cnt;

    // uint16_t __obj_size;

    Slab() = default;
    Slab(uint8_t = 0, uint16_t = 0x1000, uint64_t = -1, uint8_t* = nullptr);

    // obj_size
    // Slab(uint16_t, uint8_t, )
    //~Slab();

    uint64_t __alloc(uint16_t);
    void __free(uint16_t, uint16_t);
};

struct Cache {
    uint16_t __obj_size;
    uint16_t __full_objCnt;
    uint8_t __newSlab_flags;

    klist<Slab> full;
    klist<Slab> empty;
    klist<Slab> partial;

    void* alloc_(uint16_t, uint8_t = 0, bool = false);
    void free_(const void*);
};

struct Common_Cache_Header {
    Slab* slab;
    uint16_t len;
    const char magic[6] = "_SLAB";

    Common_Cache_Header(Slab* _slab, uint16_t _len) : slab(_slab), len(_len) {}
};

typedef linked_container<Slab> slab_container;
slab_container* __alloc_slab_container(uint8_t, uint16_t);
void __free_slab_container(slab_container*);

#endif
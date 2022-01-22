#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "KTL\bitset.h"
#include "KTL\list.h"
#include "buddy.h"
#include "kmem.h"
#include "memory.h"
#include "new.h"
#include "stdint.h"

#define SLAB_PERMANENT 0b10000000

#pragma region backup
// every slab have 4096(1 page) bytes
// struct Slab {
//     uint8_t flags;
//     BitMap bitmap;
//     uint64_t vaddr;
//     uint16_t objFree;

//     Slab();
//     Slab(uint8_t, uint32_t, uint64_t, uint8_t*);
// };

// struct Slab_cache {
//     uint32_t objSize;
//     list<Slab*> slabs_full;
//     list<Slab*> slabs_available;

//     void get_slab();

//     void* __alloc(uint16_t, bool = false);
//     void __free(Slab*, uint64_t, uint16_t len);
// };

// struct SlabAllocHeader {
//     Slab* slab;
//     uint16_t len;
//     const char magic[6] = "_SLBA";
// };

// extern Slab iSlab_0;
// extern Slab iSlab_1;

// extern linked_container<Slab*> iSlab_lnode_0;
// extern linked_container<Slab*> iSlab_lnode_1;

// extern Slab_cache common_cache;
// extern Slab_cache __Slab_cache;

// void __slab_free(Alloc_Header*);

// Slab* alloc_slab(uint8_t, uint32_t);
#pragma endregion

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

    list<Slab*> full;
    list<Slab*> empty;
    list<Slab*> partial;

    void* alloc_(uint16_t, uint8_t = 0);
    void free_();
};

Slab* __alloc_Slab(uint8_t = 0, uint16_t = 0x1000);

#endif
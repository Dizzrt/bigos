#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "mmu\memdef.h"
#include "ktl\kbitset.h"
#include "ktl\klist.h"

class Cache; // forward declaration
class Slab :public kbitset
{
public:
    uint64_t base;
    uint64_t stepSize;

    Cache* cache;
    //uint64_t colour_off; 

    uint32_t flags;
    //kbitset->_len (max number of objs)
    const uint32_t& freeObjs = _fc;

    void* alloc_slab(); //return a obj's address
    void free_slab(const void* p);

    // bp_pointer == bitmap pointer
    Slab(uint32_t ObjCnt, uint64_t StepSize, uint32_t Flags, uint64_t Base, uint8_t* Bp_pointer);
    Slab(uint32_t SlabOrder, uint32_t ObjCnt, uint64_t StepSize, uint32_t Flags, uint64_t Base, uint8_t* Bp_pointer);


    ~Slab() = default;
};

struct SlabHeader
{
    Slab* slab;
    const uint64_t magic;

    SlabHeader(Slab* _Slab);
};
#define SHSIZE sizeof(SlabHeader)
#define SH_MAGIC 0x50b7ff2785ff7b22

class Cache {
private:
    void NewSlab(klist<Slab*>& list);
public:
    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partital;

    uint32_t flags;
    uint32_t objSize;  // the obj's size in byte
    uint32_t slabOrder; //index of buddyChunkPages, indicates how many pages per slab

    // size == LONG_ALIGN(objSize+SHSIZE) 
    uint32_t size;

    uint32_t ops; //objs per slab

    void* alloc_cache();

    Cache() = default;
    ~Cache() = default;

    //SSC => static slab count
    Cache(uint32_t ObjSize, uint32_t Flags, uint32_t SSC, ...);
    Cache(uint32_t ObjSize, uint32_t Flags, uint32_t SlabOrder, uint32_t SSC, ...);
};

class ChacheChain {
private:
public:
    klist<Cache*> cache_list;
    ChacheChain() = default;
    ~ChacheChain() = default;

    //Size -- how much space need in byte
    void* alloc(uint32_t Size);

    void __insert_cache__(Cache* _cache);
    void __insert_cache__(linked_container<Cache*>* _lcache);
    Cache* __create_cache__(uint32_t Flags, uint32_t ObjSize);
};


#endif //__BIG_SLAB_H__
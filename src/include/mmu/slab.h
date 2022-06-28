#ifndef __BIG_SLAB_H__
#define __BIG_SLAB_H__

#include "mmu\memdef.h"
#include "ktl\kbitset.h"
#include "ktl\klist.h"



//slab flags
#define SLAB_PERMANENT 0x00000001
//slab flags end

//cache flags
#define CACHE_NOEMPTY 0x00000001
//cache flags end




#define LONG_ALIGN(SIZE) \
  ((SIZE + sizeof(long) - 1) & ~(sizeof(long) - 1))


#pragma region backup

// class Slab : public kbitset
// {
// private:
//     uint64_t base;
//     uint64_t offsetStep;
// public:
//     uint32_t flags;
//     const uint64_t& freeObjs = _fc;

//     void* alloc_slab();

//     //offset
//     void free_slab(uint64_t);

//     void free_slab(const void*);

//     Slab(/* args */) = default;
//     Slab(uint32_t, uint32_t, uint64_t, uint64_t, uint8_t*);


//     ~Slab() = default;
// };


// struct SlabHeader
// {
//     Slab* slab;
//     const uint64_t magic;

//     SlabHeader(Slab*);
// };
// #define SHSIZE sizeof(SlabHeader)
// #define SH_magic 0x50b7ff2785ff7b22

// enum SlabSize {
//     x1 = 1 * 0x1000, x2 = 2 * 0x1000, x4 = 4 * 0x1000,
//     x8 = 8 * 0x1000, x16 = 16 * 0x1000, x32 = 32 * 0x1000,
//     x64 = 64 * 0x1000, x128 = 128 * 0x1000, x256 = 256 * 0x1000,
//     x512 = 512 * 0x1000, x1024 = 1024 * 0x1000
// };


// class Cache
// {
// private:
//     klist<Slab*> full;
//     klist<Slab*> empty;
//     klist<Slab*> partial;

//     uint32_t _alignment_;
// public:
//     uint32_t flags;
//     uint32_t objSize;
//     // uint32_t slabSize;
//     SlabSize slabSize;


//     uint32_t ops; //objs per slab
//     uint32_t offsetStep;


//     Cache(/* args */) = default;
//     ~Cache() = default;

//     Cache(uint32_t, uint32_t, SlabSize, uint32_t, ...);

//     void* alloc_Cache();
// };


// class CacheChain
// {
// private:
//     klist<Cache*> cache_list;
// public:
//     CacheChain(/* args */) = default;
//     ~CacheChain() = default;

//     void* alloc(uint64_t);

//     void insert(Cache*);
//     void insert(linked_container<Cache*>*);

//     //void CreateCache();
// };

#pragma endregion


class Slab :public kbitset
{
private:
    uint64_t base;

    //size == objSize+SHSIZE (size of SlabHeader)
    uint32_t size;

    //uint64_t colour_off; 
public:
    uint32_t flags;
    //kbitset->_len (max number of objs)
    const uint64_t& freeObjs = _fc;

    void* alloc_slab(); //return a obj's address
    void free_slab(const void*);

    Slab() = default;
    // bp_pointer == bitmap pointer
    Slab(uint32_t Flags, uint32_t objSize, uint32_t objCnt, uint64_t base, uint8_t* bp_pointer);

    ~Slab() = default;
};

struct SlabHeader
{
    Slab* slab;
    const uint64_t magic;

    SlabHeader(Slab*);
};
#define SHSIZE sizeof(SlabHeader)
#define SH_magic 0x50b7ff2785ff7b22

class Cache {
public:
    klist<Slab*> full;
    klist<Slab*> empty;
    klist<Slab*> partital;

    uint32_t flags;
    uint32_t objSize;  // the obj's size in byte
    uint32_t slabSize; //index of buddyChunkPages, indicates how many pages per slab

    uint32_t ops; //objs per slab

    Cache() = default;
    ~Cache() = default;

    //SSC => static slab count
    Cache(uint32_t Flags, uint32_t ObjSize, uint32_t SSC, ...);
};

class ChacheChain {
private:
    klist<Cache*> cache_list;
public:
    ChacheChain() = default;
    ~ChacheChain() = default;

    //Size -- how much space need in byte
    void* alloc(uint32_t Size);

    void __insert_cache__(Cache*);
    void __insert_cache__(linked_container<Cache*>*);
    void __create_cache__(uint32_t Flags, uint32_t ObjSize);
};


#endif //__BIG_SLAB_H__
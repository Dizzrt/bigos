#include "mmu\slab.h"
#include "new.h"

#pragma region Slab

SlabHeader::SlabHeader(Slab* _slab) :slab(_slab), magic(SH_magic) {}

Slab::Slab(uint32_t Flags, uint32_t ObjCnt, uint64_t OffsetStep, uint64_t Base, uint8_t* Bit_ptr)
    :kbitset(Bit_ptr, ObjCnt), flags(Flags), offsetStep(OffsetStep), base(Base)
{

}

void* Slab::alloc_slab() {
    uint64_t offset = scan(1);
    set(offset);

    offset = offset * offsetStep + base;
    new ((SlabHeader*)offset) SlabHeader(this);

    return (void*)(offset + SHSIZE);
}

void Slab::free_slab(uint64_t p) {
    uint64_t offset = (p - base) / offsetStep;
    reset(offset);
}

void Slab::free_slab(const void* p) { free_slab((uint64_t)p - SHSIZE); }

#pragma endregion


#pragma region Cache

#include "stdarg.h"
//Sscnt => static slab cnt
Cache::Cache(uint32_t Flags, uint32_t ObjSize, SlabSize SSize, uint32_t Sscnt, ...) :
    flags(Flags), objSize(ObjSize), slabSize(SSize),
    ops((uint32_t)SSize / LONG_ALIGN(ObjSize)), offsetStep(LONG_ALIGN(ObjSize))
{
    va_list vlist;
    va_start(vlist, Sscnt);

    while (Sscnt--)
    {
        linked_container<Slab*>* _slc = va_arg(vlist, linked_container<Slab*>*);
        empty.__list_insert(_slc);
    }

    va_end(vlist);
}

extern Cache Cache_slab;
void* Cache::alloc_Cache() {
    if (partial.empty()) {
        if (!empty.empty()) {
            auto iter = empty.begin();
            empty.__list_remove(iter);
            partial.__list_insert(iter);
        }
        else {
            //TODO new slab

        }

        if (flags & CACHE_NOEMPTY && empty.empty()) {
            //TODO new slab to empty
        }
    }

    //list partial has at least one slab
    auto iter = partial.begin();
    Slab* s = *iter;

    void* ret = s->alloc_slab();
    if (s->freeObjs == 0) {
        partial.__list_remove(iter);
        full.__list_insert(iter);
    }

    return ret;
}

#pragma endregion


#pragma region CacheChain

void* CacheChain::alloc(uint64_t len) {
    for (auto i : cache_list)
        if (i->objSize >= len) {
            return i->alloc_Cache();
        }

    //TODO param
    return nullptr;
}

void CacheChain::insert(Cache* cache_ptr) {
    //for(auto i:)
}

void CacheChain::insert(linked_container<Cache*>* lcache_ptr) {}

#pragma endregion
#include "mmu\slab.h"
#include "new.h"

#pragma region Slab

SlabHeader::SlabHeader(Slab* _slab) :slab(_slab), magic(SH_magic) {}

Slab::Slab(uint32_t Flags, uint32_t ObjCnt, uint64_t OffsetStep, uint64_t Base, uint8_t* Bit_ptr)
    :kbitset(Bit_ptr, ObjCnt), flags(Flags), offsetStep(OffsetStep), base(Base)
{

}

void* Slab::alloc_slab() {
    uint32_t offset = scan(1);
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

void* Cache::alloc_Cache() {

}

#pragma endregion


#pragma region CacheChain

void* CacheChain::alloc(uint64_t len) {}

void CacheChain::insert(Cache* cache_ptr) {}

void CacheChain::insert(linked_container<Cache*>* lcache_ptr) {}
int a = sizeof(long);
#pragma endregion
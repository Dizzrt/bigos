#include "mmu\mflags.h"
#include "mmu\slab.h"
#include "kassert.h"
#include "stdarg.h"
#include "new.h"

extern Slab* kmemAlloc_slab();
extern Cache* kmemAlloc_Cache();
extern void* kmemAlloc_lcPonter();

void* ChacheChain::alloc(uint32_t Size) {

    for (auto i : cache_list) {
        //TODO flags
        if (i->objSize >= Size)
            return i->alloc_cache();
    }

    //have no suitable cache
    //create a new cache

    Cache* c = nullptr;
    c = kmemAlloc_Cache();

    linked_container<Cache*>* lcc =
        (linked_container<Cache*>*)kmemAlloc_lcPonter();

    //TODO flags
    new (c) Cache(Size, 0, 0);
    new (lcc) linked_container<Cache*>(c);

    cache_list.__list_insert(lcc);
    return c->alloc_cache();
}

Cache* ChacheChain::__create_cache__(uint32_t Flags, uint32_t ObjSize) {
    KAssert(ObjSize <= SLAB_OBJSIZE_LIMIT,
        "new cache's objSize must below SLAB_OBJSIZE_LIMIT");

    Cache* c = kmemAlloc_Cache();
    new (c) Cache(ObjSize, Flags, 0);

    __insert_cache__(c);

    return c;
}

void ChacheChain::__insert_cache__(linked_container<Cache*>* _lcache) {
    Cache* t = _lcache->val;

    auto iter = cache_list.begin();
    while (true)
    {
        if (iter == cache_list.end()) {
            cache_list.__list_insert(_lcache);
            break;
        }
        else if ((*iter)->objSize < t->objSize) {
            iter++;
            continue;
        }
        else {
            cache_list.__list_insert(_lcache, iter);
            break;
        }
    }
    return;
}

void ChacheChain::__insert_cache__(Cache* _cache) {
    linked_container<Cache*>* _lc =
        (linked_container<Cache*>*)kmemAlloc_lcPonter();

    new (_lc) linked_container<Cache*>(_cache);

    __insert_cache__(_lc);
    return;
}

void Cache::NewSlab(klist<Slab*>& list) {
    Slab* _slab = kmemAlloc_slab();
    linked_container<Slab*>* _lc =
        (linked_container<Slab*>*)kmemAlloc_lcPonter();

    new (_slab) Slab(slabOrder, ops, size, SLAB_DEFAULT, 0, nullptr);
    new (_lc) linked_container<Slab*>(_slab);

    list.__list_insert(_lc);
}

void* Cache::alloc_cache() {
    if (partital.empty()) {
        if (!empty.empty()) {
            auto iter = empty.begin();
            empty.__list_remove(iter);
            partital.__list_insert(iter);

            if (flags & __CACHE_NONEMPTY
                && empty.empty()) {

                NewSlab(empty);
            }
        }
        else {
            NewSlab(empty);
            NewSlab(partital);
        }
    }

    //partial list has at least one slab
    auto iter = partital.begin();
    Slab* s = *iter;

    void* ret = s->alloc_slab();
    if (s->freeObjs == 0) {
        partital.__list_remove(iter);
        full.__list_insert(iter);
    }

    return ret;
}

Cache::Cache(uint32_t ObjSize, uint32_t Flags, uint32_t SSC, ...)
    : objSize(ObjSize), flags(Flags), size(LONG_ALIGN(ObjSize + SHSIZE))
{
    uint32_t temp, delta = INT32_MAX;
    for (uint32_t i = 0;i < BUDDY_MAX_ORDER;i++) {
        temp = buddyChunkSize[i] - (buddyChunkSize[i] / size) * size;

        if (temp < delta) {
            delta = temp;
            slabOrder = i;
        }
    }

    ops = buddyChunkSize[slabOrder] / size;

    va_list vlist;
    va_start(vlist, SSC);

    while (SSC--)
    {
        linked_container<Slab*>* _lc = va_arg(vlist, linked_container<Slab*>*);
        empty.__list_insert(_lc);
    }

    va_end(vlist);
}

Cache::Cache(uint32_t ObjSize, uint32_t Flags, uint32_t SlabOrder, uint32_t SSC, ...)
    : objSize(ObjSize), flags(Flags), size(LONG_ALIGN(ObjSize + SHSIZE)),
    slabOrder(SlabOrder), ops(buddyChunkSize[slabOrder] / size)
{
    va_list vlist;
    va_start(vlist, SSC);

    while (SSC--)
    {
        linked_container<Slab*>* _lc = va_arg(vlist, linked_container<Slab*>*);
        empty.__list_insert(_lc);

        _lc->val->cache = this;
    }

    va_end(vlist);
}

//TODO adjust
Slab::Slab(uint32_t SlabOrder, uint32_t ObjCnt, uint64_t StepSize,
    uint32_t Flags, uint64_t Base, uint8_t* Bp_pointer)
    : kbitset(Bp_pointer == nullptr ? nullptr/*TODO alloc Bp_pointer*/ : Bp_pointer, ObjCnt),
    base(Base == 0 ? 0/*TODO alloc base*/ : Base), flags(Flags), stepSize(StepSize) {}

void* Slab::alloc_slab() {
    uint64_t offset = scan(1);
    set(offset);

    offset = offset * stepSize + base;
    new ((SlabHeader*)offset) SlabHeader(this);

    return (void*)(offset + SHSIZE);
}

void Slab::free_slab(const void* p) {
    uint64_t offset = (uint64_t)p;
    offset = (offset - SHSIZE - base) / stepSize;

    if (cache != nullptr) {
        klist<Slab*>::iterator iter;

        if (!_fc)
            iter = cache->full.begin();
        else iter = cache->partital.begin();

        while (true)
        {
            if ((*iter) == this)
                break;

            else iter++;
        }

        if (!_fc) {
            cache->full.__list_remove(iter);

            if (_fc + 1 == _len)
                cache->empty.__list_insert(iter);
            else cache->partital.__list_insert(iter);
        }
        else if (_fc + 1 == _len) {
            cache->partital.__list_remove(iter);
            cache->empty.__list_insert(iter);
        }
    }

    reset(offset);
}

SlabHeader::SlabHeader(Slab* _Slab)
    :slab(_Slab), magic(SH_MAGIC) {}
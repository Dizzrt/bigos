#include "mmu\slab.h"
#include "new.h"
#include "stdarg.h"

extern Cache cache_slab;
extern Cache cache_lcPointer;

uint64_t SH_magic;

Slab::Slab(uint8_t _flags, uint16_t _objSize, uint64_t _page, uint8_t* _bp)
    : offsetSize(LONG_ALIGN(_objSize + SHSIZE)), flags(_flags), page(_page),
    kbitset(_bp, 0x1000 / LONG_ALIGN(_objSize + SHSIZE)) {}

//scount => number of static slab
Cache::Cache(uint8_t _flags, uint16_t _objSize, uint32_t scount, ...) : flags(_flags), objSize(_objSize) {
    va_list vlist;
    va_start(vlist, scount);
    while (scount--) {
        linked_container<Slab*>* _slc = va_arg(vlist, linked_container<Slab*>*);
        empty.__list_insert(_slc);
    }
    va_end(vlist);
}

void Slab::__free(uint64_t p) {
    if (p < page || p > page + 0x1000)
        return;

    if (((SlabHeader*)p)->magic != SH_magic)
        return;

    uint64_t offset = (p - page) / offsetSize;

    reset(offset);
}

void Slab::__free(const void* p) { __free((uint64_t)p - SHSIZE); }

void* Slab::__alloc() {
    uint64_t offset = scan(1);
    set(offset);

    offset = offset * offsetSize + page;
    new ((SlabHeader*)offset) SlabHeader(this);

    return (void*)(offset + SHSIZE);
}

void* Cache::_alloc() {
    if (partial.empty()) {
        if (empty.empty()) {
            linked_container<Slab*>* _lcs0 = (linked_container<Slab*>*)cache_lcPointer._alloc();
            linked_container<Slab*>* _lcs1 = (linked_container<Slab*>*)cache_lcPointer._alloc();
            Slab* _s0 = (Slab*)cache_slab._alloc();
            Slab* _s1 = (Slab*)cache_slab._alloc();

            new (_s0) Slab(dffs, objSize, (uint64_t)buddy_alloc(1));
            new (_s1) Slab(dffs, objSize, (uint64_t)buddy_alloc(1));

            _lcs0->val = _s0;
            _lcs1->val = _s1;

            empty.__list_insert(_lcs0);
            partial.__list_insert(_lcs1);

        }
        else {
            klist<Slab*>::iterator iter = empty.begin();
            empty.__list_remove(iter);
            partial.__list_insert(iter.m_node);

            if ((flags & CACHE_NONEMPTY) && empty.empty()) {
                for (int i = 0; i < 2; i++) {
                    linked_container<Slab*>* _lcs = (linked_container<Slab*>*)cache_lcPointer._alloc();
                    Slab* _s = (Slab*)cache_slab._alloc();

                    new(_s) Slab(dffs, objSize, (uint64_t)buddy_alloc(1));

                    _lcs->val = _s;
                    empty.__list_insert(_lcs);
                }
            }
        }
    }
    klist<Slab*>::iterator iter = partial.begin();
    Slab* _slab = *iter;
    void* ret = _slab->__alloc();

    if (_slab->freeObjs == 0) {
        partial.__list_remove(iter);
        full.__list_insert(iter.m_node);
    }

    return ret;
}

void* ::CacheChain::alloc(uint64_t size) {
    klist<Cache*>::iterator iter = _caList.begin();
    do {
        if ((*iter)->objSize >= size)
            return (*iter)->_alloc();

        iter++;
    } while (iter != _caList.end());

    return nullptr;
}

void CacheChain::insert(linked_container<Cache*>* _cache) {
    if (_caList.empty()) {
        _caList.__list_insert(_cache);
        return;
    }

    klist<Cache*>::iterator iter = _caList.begin();
    do {
        if ((*iter)->objSize > _cache->val->objSize) {
            _caList.__list_insert(_cache, iter);
            return;
        }
        iter++;
    } while (iter != _caList.end());

    _caList.__list_insert(_cache);
}
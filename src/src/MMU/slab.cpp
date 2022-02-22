#include "mmu\slab.h"
#include "new.h"

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
            // TODO alloc new slab
        } else {
            klist<Slab*>::iterator iter = empty.begin();
            empty.__list_remove(iter);
            partial.__list_insert(iter.m_node);
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
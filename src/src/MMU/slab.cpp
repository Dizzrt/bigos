#include "MMU\slab.h"

// slab can't include buddy.h and kmem.h,use extern to declare what we want
extern Cache slab_cache;
extern Cache common_cache;

extern void* __buddy_alloc(uint32_t = 1);
extern void __buddy_free(void*);
extern void* kmalloc(uint32_t, uint8_t = 0);
extern void kfree(const void*);

uint64_t slabHeaderMagic = 0xa3188ec947cb43f8;  // TODO update slab header magic

/**
 * @description:
 * @param {uint8_t} flags
 * @param {uint16_t} objs' count
 * @param {uint64_t} page
 * @param {uint8_t*} bitset pointer
 */
Slab::Slab(uint8_t flags, uint16_t objCnt, uint64_t page, uint8_t* bp)
    : bitset((bp == nullptr ? (uint8_t*)kmalloc(sizeof(objCnt)) : bp), objCnt) {
    __flags = flags;
    __free_obj_cnt = objCnt;

    if (page == -1)
        __page = (uint64_t)__buddy_alloc();
    else
        __page = page;
}

uint64_t Slab::__alloc() {
    uint64_t offset = scan(1);
    __free_obj_cnt--;
    set(offset, 1);
    return offset;
}

void Slab::__free(const void* p) {
    uint64_t offset = ((uint64_t)p - __page) / _objSize;
    reset(offset, 1);
    __free_obj_cnt++;
}

void* Cache::_alloc() {
    if (partial.empty())
        if (empty.empty()) {
            // TODO alloc new slab
        } else {
            klist<Slab>::iterator iter = empty.begin();
            empty.__list_remove(iter);
            partial.__list_insert(iter.m_node, partial.end());
        }

    klist<Slab>::iterator iter = partial.begin();
    uint64_t ret = (*iter).__page + (*iter).__alloc() * _objSize;

    if ((*iter).__free_obj_cnt <= 0) {
        partial.__list_remove(iter);
        full.__list_insert(iter.m_node, full.end());
    }

    new ((SlabHeader*)ret) SlabHeader(&(*iter));
    return (void*)(ret + sizeof(SlabHeader));
}

void* CacheChain::alloc(uint16_t objSize) {
    klist<Cache>::iterator iter = _caList.begin();
    while (iter != _caList.end()) {
        if ((*iter)._objSize >= objSize)
            return (*iter)._alloc();

        iter++;
    }

    // TODO cache alloc
    return nullptr;
}
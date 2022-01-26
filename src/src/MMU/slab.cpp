#include "MMU\slab.h"

// slab can't include buddy.h and kmem.h,use extern to declare what we want
extern Cache slab_cache;
extern Cache common_cache;

extern void* __buddy_alloc(int = 1);
extern void __buddy_free(void*);
extern void* kmalloc(uint32_t, uint8_t = 0);
extern void kfree(const void*);

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

uint64_t Slab::__alloc(uint16_t obj_cnt) {
    uint64_t ret = scan(obj_cnt);
    set(ret, obj_cnt);
    __free_obj_cnt -= obj_cnt;
    return ret;
}

void Slab::__free(uint16_t _offset, uint16_t obj_cnt) {
    reset(_offset, obj_cnt);
    __free_obj_cnt += obj_cnt;
}

void* Cache::alloc_(uint16_t obj_cnt, uint8_t flags, bool isCommon) {
    // TODO alloc_ flags
    Slab* _slab;
    if (partial.empty()) {
        while (empty.size() < 2) {
            slab_container* temp = __alloc_slab_container(this->__newSlab_flags, this->__full_objCnt);
            empty.__list_insert(temp, empty.end());
        }

        slab_container* temp = empty.begin().m_node;
        empty.__list_remove(empty.begin());
        partial.__list_insert(temp, partial.end());
    }

    klist<Slab>::iterator iter = partial.begin();
    while (iter != partial.end()) {
        _slab = &*iter;
        if (_slab->__free_obj_cnt < obj_cnt) {
            iter++;
            continue;
        }

        uint64_t ret = _slab->__alloc(obj_cnt);
        if (ret != -1) {
            ret = ret * __obj_size + _slab->__page;

            if (isCommon) {
                new ((Common_Cache_Header*)ret) Common_Cache_Header(_slab, obj_cnt);
                ret -= sizeof(Common_Cache_Header);
            }

            return (void*)ret;
        }
    }

    return nullptr;
}

void Cache::free_(const void*) {}

slab_container* __alloc_slab_container(uint8_t flags, uint16_t objCnt) {
    slab_container* sc = (slab_container*)slab_cache.alloc_(1);
    new (&sc->val) Slab(flags, objCnt);
    return sc;
}

void __free_slab_container(slab_container*) {
    // TODO free slab container
    //
}
/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-12 17:38:55
 */

#include "MMU\slab.h"

void *Slab_cache::__alloc(uint16_t len) {
    if (slabs_available.empty()) {
        // TODO get new slab
        return nullptr;
    }

    list<Slab *>::iterator iter = slabs_available.begin();
    do {
        Slab *_slab = *iter;

        if (_slab->objFree < len)
            continue;

        uint64_t offset = bitmap_scan(&_slab->bitmap, len);
        if (offset == -1)
            continue;

        bitmap_update(&_slab->bitmap, offset, len);
        return (void *)(_slab->vaddr + _slab->objSize * offset);

    } while (++iter != slabs_available.end());

    return nullptr;
}

Slab::Slab() {
    this->flags = 0;
    this->vaddr = 0;
    this->objSize = 1;
    this->objFree = 4096;
    this->bitmap.len = 4096;
    this->bitmap.bits = nullptr;
}

Slab::Slab(uint8_t _flag, uint32_t _objSize, uint64_t _vaddr, uint8_t *_bp) {
    this->flags = _flag;
    this->objSize = _objSize;
    this->objFree = 4096 / _objSize;
    this->vaddr = _vaddr;

    this->bitmap.len = this->objFree;
    this->bitmap.bits = _bp;
}
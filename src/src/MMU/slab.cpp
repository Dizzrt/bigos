/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-13 18:13:30
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
        _slab->objFree -= len;
        if (_slab->objFree == 0) {
            // TODO move to full list
        }
        return (void *)(_slab->vaddr + objSize * offset);

    } while (++iter != slabs_available.end());

    return nullptr;
}

Slab::Slab() {
    this->flags = 0;
    this->vaddr = 0;
    this->objFree = 4096;
    this->bitmap.len = 4096;
    this->bitmap.bits = nullptr;
}

Slab::Slab(uint8_t _flag, uint32_t _objSize, uint64_t _vaddr, uint8_t *_bp) {
    this->flags = _flag;
    this->objFree = 4096 / _objSize;
    this->vaddr = _vaddr;

    this->bitmap.len = this->objFree;
    this->bitmap.bits = _bp;
}
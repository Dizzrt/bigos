/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 20:44:01
 */

#include "MMU\slab.h"

void *Slab_cache::__alloc(size_t len) {
    if (len > 0x1000)
        return nullptr;

    if (pool_available.empty()) {
        // TODO get new slab from buddy
    }

    uint64_t offset;
    list<Slab *>::iterator iter = pool_available.begin();

    while (iter != pool_available.end()) {

        if ((*iter)->__free < len)
            goto flag;

        offset = bitmap_scan(&(*iter)->bitmap, len);

        if (offset != -1) {
            bitmap_update(&(*iter)->bitmap, offset, len);
            return (void *)((*iter)->vaddr + offset);
        }

    flag:
        iter++;
    }

    return nullptr;
}

void Slab_cache::appendSlab(Slab *slab) {}

void Slab_cache::__appendSlab_(__list_node<Slab *> *node) { pool_available.__push_back_(node); }

void Slab_cache::__tmporary_init() {
    pool_full.__temporary_init_();
    pool_available.__temporary_init_();
}

Slab::Slab() {
    this->vaddr = 0;
    this->__using = 0;

    this->__free = 4096;
    this->bitmap.len = 4096;

    this->bitmap.bits = nullptr;
    this->type = SlabType::NORMAL;
}

Slab::Slab(SlabType _type, uint64_t _vaddr, uint8_t *_bp) : Slab::Slab() {
    this->type = _type;
    this->vaddr = _vaddr;
    this->bitmap.bits = _bp;
}

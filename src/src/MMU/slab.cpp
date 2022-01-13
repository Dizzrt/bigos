#include "MMU\slab.h"
#include "MMU\buddy.h"

//----static slab----
Slab iSlab_0;
Slab iSlab_1;

linked_container<Slab*> iSlab_lnode_0;
linked_container<Slab*> iSlab_lnode_1;

Slab_cache common_cache;
Slab_cache __Slab_cache;
//-------------------

void* Slab_cache::__alloc(uint16_t len, bool hasHeader) {
    if (slabs_available.empty()) {
        Slab* _slab = alloc_slab(0, this->objSize);
        slabs_available.push_back(_slab);
    }

    list<Slab*>::iterator iter = slabs_available.begin();
    do {
        Slab* _slab = *iter;

        if (_slab->objFree < len)
            continue;

        uint64_t offset = bitmap_scan(&_slab->bitmap, len);
        if (offset == -1)
            continue;

        bitmap_update(&_slab->bitmap, offset, len);

        _slab->objFree -= len;
        if (_slab->objFree == 0) {
            slabs_available.__list_rm(iter);
            slabs_full.__list_add(iter.m_node, slabs_full.end());
        }

        uint64_t ret = _slab->vaddr + objSize * offset;
        if (hasHeader) {
            SlabAllocHeader* header = (SlabAllocHeader*)ret;
            header->len = len;
            header->slab = _slab;

            ret += sizeof(SlabAllocHeader);
        }

        return (void*)ret;
    } while (++iter != slabs_available.end());

    return nullptr;
}

void Slab_cache::__free(Slab* _slab, uint64_t offset, uint16_t len) {
    bitmap_update(&_slab->bitmap, offset, len, false);
    _slab->objFree += len;

    for (list<Slab*>::iterator iter = slabs_full.begin(); iter != slabs_full.end(); iter++)
        if ((*iter) == _slab) {
            slabs_full.__list_rm(iter);
            slabs_available.__list_add(iter.m_node, slabs_available.end());
            break;
        }
}

Slab::Slab() {
    this->flags = 0;
    this->vaddr = 0;
    this->objFree = 4096;
    this->bitmap.len = 4096;
    this->bitmap.bits = nullptr;
}

Slab::Slab(uint8_t _flag, uint32_t _objSize, uint64_t _vaddr, uint8_t* _bp) {
    this->flags = _flag;
    this->objFree = 4096 / _objSize;
    this->vaddr = _vaddr;

    this->bitmap.len = this->objFree;
    this->bitmap.bits = _bp;
}

void __slab_free(Alloc_Header* header) {
    Slab* _slab = (Slab*)header->_allocator;
    bitmap_update(&_slab->bitmap, header->offset, header->len, false);
    _slab->objFree += header->len;
}

Slab* alloc_slab(uint8_t _flags, uint32_t _objSize) {
    while (__Slab_cache.slabs_available.size() < 2) {
        Slab* _slab = (Slab*)__Slab_cache.__alloc(1);
        _slab->flags = 0;
        _slab->objFree = 0x1000 / sizeof(Slab);
        _slab->vaddr = (uint64_t)alloc_pages();
        _slab->bitmap.bits = _slab->bitmap.bits =
            (uint8_t*)kmalloc(_slab->objFree / 8 + (_slab->objFree % 8 == 0 ? 0 : 1));
        _slab->bitmap.len = _slab->objFree;
        bitmap_init(&_slab->bitmap);
        __Slab_cache.slabs_available.push_back(_slab);
    }

    Slab* _slab = (Slab*)__Slab_cache.__alloc(1);
    _slab->flags = _flags;
    _slab->objFree = 0x1000 / _objSize;
    _slab->vaddr = (uint64_t)alloc_pages();
    _slab->bitmap.bits = (uint8_t*)kmalloc(_slab->objFree / 8 + (_slab->objFree % 8 == 0 ? 0 : 1));
    _slab->bitmap.len = _slab->objFree;
    bitmap_init(&_slab->bitmap);

    return _slab;
}
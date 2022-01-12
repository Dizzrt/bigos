#ifndef __BIG_KTL_ALLOCATOR_H__
#define __BIG_KTL_ALLOCATOR_H__

#include "MMU\slab.h"

template <typename T>
struct ktl_allocator {
  private:
    Slab_cache ktl_allocator_cache;
    ktl_allocator() { ktl_allocator_cache.objSize = sizeof(T) + SlabAllocHeader; }

  public:
    T* alloc() { return (T*)ktl_allocator_cache.__alloc(1); }

    void free(const T* obj) {
        uint64_t addr = (uint64_t)obj - sizeof(SlabAllocHeader);
        SlabAllocHeader* header = (SlabAllocHeader*)addr;

        if (strcmp(header->magic, "_SLAB"))  // not a valid addr
            return;

        bitmap_update(&header->slab->bitmap, addr - header->slab->vaddr, header->len, false);
        header->slab->objFree++;
    }
};

#endif
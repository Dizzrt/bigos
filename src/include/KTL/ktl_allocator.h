#ifndef __BIG_KTL_ALLOCATOR_H__
#define __BIG_KTL_ALLOCATOR_H__

#include "MMU\slab.h"

template <typename T>
struct ktl_allocator {
  private:
    slab_cache ktl_allocator_cache;

    // TODO ktl allocator init
    ktl_allocator() { ktl_allocator_cache.objSize = sizeof(T) + SlabAllocHeader; }

  public:
    T& alloc() { return ktl_allocator_cache.__alloc(1); }

    void free(const T& obj) {}
};

#endif
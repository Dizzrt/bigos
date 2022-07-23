#ifndef __BIG_KTL_ALLOCATOR_H__
#define __BIG_KTL_ALLOCATOR_H__

#include "stddef.h"

template<typename _Tp>
class __kallocator_base {
public:
    typedef _Tp* pointer;


    virtual pointer allocate(size_t __n) noexcept = 0;
    virtual void deallocate(pointer __p) noexcept = 0;
    virtual void construct() noexcept = 0;
    virtual void destory() noexcept = 0;
};



#endif
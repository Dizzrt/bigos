#ifndef __BIG_KTL_ALLOCATOR_H__
#define __BIG_KTL_ALLOCATOR_H__

#include "stddef.h"
#include "c++\move.h"
#include "io.h"

namespace kstd
{
    template<typename _Tp>
    class __kallocator_base {
    public:
        typedef _Tp* pointer;


        virtual pointer allocate(size_t __n) noexcept = 0;
        virtual void deallocate(pointer __p) noexcept = 0;

        template<typename _Up, typename... _Args>
        void construct(_Up* __p, _Args&&... __args) {
            ::new((void*)__p) _Up(std::forward<_Args>(__args)...);
        }

        template<typename _Up>
        void destroy(_Up* __p) { __p->~_Up(); }
    };

    template<typename _Tp>
    class allocator :public __kallocator_base<_Tp> {
    public:
        typedef _Tp* pointer;

        pointer allocate(size_t __n) noexcept {
            printk_svga("allocate\n");
            return nullptr;//TODO
        }

        void deallocate(pointer __p)noexcept {

        }
    };

    template<typename _Tp,
        template<typename> class _Alloc>
    struct Allocator {
    private:
        _Alloc<_Tp> __alloc;
    public:
        // __kallocator_base<_Tp>& allocator = __alloc;

        inline _Tp* allocate(size_t __n) { return __alloc.allocate(__n); }
        inline void deallocate(_Tp* __p) { __alloc.deallocate(__p); }

        template<typename _Up, typename... _Args>
        inline void construct(_Up* __p, _Args&&... __args) {
            __alloc.construct(__p, std::forward<_Args>(__args)...);
        }

        template<typename _Up>
        inline void destroy(_Up* __p) { __alloc.destory(__p); }
    };

} // namespace kstd

#endif
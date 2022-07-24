#ifndef __BIG_MOVE_H__
#define __BIG_MOVE_H__

#include "type_traits.h"

namespace std {
    //forward an lvalue
    template<typename _Tp> constexpr _Tp&&
        forward(typename std::remove_reference<_Tp>::type& __t)noexcept {
        return static_cast<_Tp&&>(__t);
    }

    //forward an rvalue
    template<typename _Tp> constexpr _Tp&&
        forward(typename std::remove_reference<_Tp>::type&& __t) noexcept {
        static_assert(!std::is_lvalue_reference<_Tp>::value,
            "template argument substituting _Tp is an lvalue reference type");
        return static_cast<_Tp&&>(__t);
    }

    //convert a value to rvalue
    template<typename _Tp>
    constexpr typename std::remove_reference<_Tp>::type&&
        move(_Tp&& __t)noexcept {
        return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);
    }
}//namespace

#endif //__BIG_MOVE_H__
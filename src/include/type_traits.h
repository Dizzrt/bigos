#ifndef __BIG_TYPE_TRAITS_H__
#define __BIG_TYPE_TRAITS_H__

template <typename _Tp, _Tp __v>
struct integral_constant {
    static constexpr _Tp value = __v;
    typedef _Tp value_type;
    typedef integral_constant<_Tp, __v> type;
    constexpr operator value_type() const noexcept { return value; }
#if __cplusplus > 201103L

#define __cpp_lib_integral_constant_callable 201304

    constexpr value_type operator()() const noexcept { return value; }
#endif
};

typedef integral_constant<bool, true> true_type;

/// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false> false_type;

template <typename, typename>
struct is_same : public false_type {};

template <typename _Tp>
struct is_same<_Tp, _Tp> : public true_type {};

template <typename _Tp>
struct __declval_protector {
    static const bool __stop = false;
};

template <typename _Tp, typename _Up = _Tp&&>
_Up __declval(int);

template <typename _Tp>
_Tp __declval(long);

template <typename _Tp>
auto declval() noexcept -> decltype(__declval<_Tp>(0));

template <typename _Tp>
auto declval() noexcept -> decltype(__declval<_Tp>(0)) {
    static_assert(__declval_protector<_Tp>::__stop, "declval() must not be used!");
    return __declval<_Tp>(0);
}

template <typename T>
struct has_equal_operator {
    template <typename U>
    static auto test(int) -> decltype(declval<U>() == declval<U>());

    // template<typename U> static auto test(int)->	decltype(declval<U>().operator==(declval<U>()));

    template <typename U>
    static void test(...);

    enum { value = is_same<decltype(test<T>(0)), bool>::value };
};

#endif
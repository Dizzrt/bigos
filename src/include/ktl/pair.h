#ifndef __BIG_PAIR_H__
#define __BIG_PAIR_H__

template <typename T1, typename T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& p) :pair(p.first, p.second) {}
    pair() : pair(T1(), T2()) {}
};

template <typename T1, typename T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first == y.first && x.second == y.second;
}

template <typename T1, typename T2>
inline bool operator==(const pair<T1, T2>& x, const T1& key) {
    return x.first == key;
}

template <typename T1, typename T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
    return x.first < y.first || (!(y.first < x.first) && x.second > y.second);
}

template <typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
    return pair<T1, T2>(x, y);
}

#endif
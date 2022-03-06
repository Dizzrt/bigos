#ifndef __BIG_ALGORITHEM_H__
#define __BIG_ALGORITHEM_H__

template<typename T>
void swap(T& x, T& y) {
    T temp = x;
    x = y, y = temp;
}

#endif
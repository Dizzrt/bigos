#ifndef __BIG_BITSET_H__
#define __BIG_BITSET_H__

#include "stdint.h"

class bitset {
  protected:
    uint8_t* _bp;  // bit pointer
    uint64_t _size;

  public:
    bitset() = default;
    bitset() : _size(8) { resize(_size); };
    bitset(uint8_t* bp, uint64_t __Size) : _bp(bp), _size(__Size){};
    //~bitset(); //MARKER destructor

    void set(uint64_t);
    void set(uint64_t, uint64_t);
    void reset(uint64_t);
    void reset(uint64_t, uint64_t);

    void fill() { set(0, _size); }
    void clear() { reset(0, _size); };

    void resize(uint64_t);

    uint64_t scan(uint64_t size);
};

void bitset::set(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp |= mask;
}

void bitset::reset(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp &= ~mask;
}

void bitset::set(uint64_t begin, uint64_t size) {
    uint8_t* bp = _bp + begin / 8;

    uint8_t _s = begin % 8;
    uint8_t mask = 0x80 >> _s;

    _s = 8 - _s;
    if (size <= _s)
        _s = size;
    size -= _s;

    while (_s--) {
        *bp |= mask;
        mask >>= 1;
    }
    bp++;

    _s = size / 8;
    while (_s--) {
        *bp = 0xff;
        bp++;
    }
}

void bitset::reset(uint64_t begin, uint64_t size) {
    uint8_t* bp = _bp + begin / 8;

    uint8_t _s = begin % 8;
    uint8_t mask = 0x80 >> _s;

    _s = 8 - _s;
    if (size <= _s)
        _s = size;
    size -= _s;

    while (_s--) {
        *bp &= ~mask;
        mask >>= 1;
    }
    bp++;

    _s = size / 8;
    while (_s--) {
        *bp = 0;
        bp++;
    }
}

void bitset::resize(uint64_t _Size) {
    // kfree(_bp);
    //_bp = (uint8_t*)kmalloc(_Size); //TODO memory

    _size = _Size;
}

uint64_t bitset::scan(uint64_t _Size) {
    uint8_t* bp = _bp;

    uint64_t _xSize;
    while (true) {
    __LOOP_FIND:
        _xSize = _Size;

        while (*bp == 0xff)
            bp++;

        uint64_t _ret = 0;
        uint8_t mask = 0xff;
        while (mask & *bp != 0 && _xSize != 0)
            mask >>= 1, _xSize--, _ret++;

        uint64_t ret = (uint64_t)(bp - _bp) * 8 + _ret;
        if (!_xSize)
            return ret;

        uint64_t temp = _xSize / 8;
        while (temp--) {
            bp++;
            if (*bp != 0)
                goto __LOOP_FIND;
        }

        bp++;
        mask = 0x80;
        temp = _xSize % 8;
        while (temp--) {
            if (*bp & mask != 0)
                goto __LOOP_FIND;
            mask >>= 1;
        }

        return ret;
    }
}

#endif
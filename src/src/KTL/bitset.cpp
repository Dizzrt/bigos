#include "KTL\bitset.h"

void bitset::set(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp |= mask;
    _fc--;
}

void bitset::reset(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp &= ~mask;
    _fc++;
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

    _fc -= size;
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

    _fc += size;
}

void bitset::resize(uint64_t _Size) {
    // kfree(_bp);
    //_bp = (uint8_t*)kmalloc(_Size); //TODO memory

    _fc = _size = _Size;
}

uint64_t bitset::scan(uint64_t _Size) {
    // TODO scan

    // single scan for now
    uint8_t* bp = _bp;
    while (*bp == 0xff)
        bp++;

    uint64_t _ret = 0;
    uint8_t mask = 0x80;

    while (mask & *bp)
        mask >>= 1, _ret++;

    return (bp - _bp) * 8 + _ret;
}
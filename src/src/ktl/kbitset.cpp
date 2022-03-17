#include "ktl\kbitset.h"

void kbitset::set(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp |= mask;
    _fc--;
}

void kbitset::reset(uint64_t _i) {
    uint8_t* bp = _bp + _i / 8;
    uint8_t mask = 0x80 >> (_i % 8);

    *bp &= ~mask;
    _fc++;
}

void kbitset::set(uint64_t begin, uint64_t size) {
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

void kbitset::reset(uint64_t begin, uint64_t size) {
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

inline uint64_t kbitset::isFree(uint64_t pos) {
    return _bp[pos / 8] >> (8 - pos % 8 - 1) & 1;
}

uint64_t kbitset::scan(uint64_t len) {
    for (int i = 0;i < _len;i++) {
        uint64_t* p_64 = (uint64_t*)(_bp + i);

    }

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

inline void kbitset::init(uint8_t* bp, uint64_t len) {
    _bp = bp;
    _fc = _len = len;
}
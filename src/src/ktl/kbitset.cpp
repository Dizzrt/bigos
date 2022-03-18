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

inline bool kbitset::isFree(uint64_t pos) {
    return !(_bp[pos / 8] >> (8 - pos % 8 - 1) & 1);
}

uint64_t kbitset::scan(uint64_t len) {
    uint16_t* p_16;
    uint32_t* p_32;
    uint64_t* p_64;

    for (uint64_t i = 0;i < _len;i++) {
        p_64 = (uint64_t*)(_bp + i);
        if (i + 64 < _len && *p_64 == 0xffffffffffffffff)
            i += 63;
        else {
            p_32 = (uint32_t*)(_bp + i);
            if (i + 32 < _len && *p_32 == 0xffffffff)
                i += 31;
            else {
                p_16 = (uint16_t*)(_bp + i);
                if (i + 16 < _len && *p_16 == 0xffff)
                    i += 15;
                else {
                    if (_bp[i / 8] == 0xff) {
                        i += 7;
                        continue;
                    }

                    if (isFree(i)) {
                        uint64_t cnt = 0, j = i;
                        while (isFree(j) && cnt < len && j < _len)
                            j++, cnt++;

                        if (cnt == len)
                            return i;
                        i = --j;
                    }
                }
            }
        }
    }

    return -1;
}

inline void kbitset::init(uint8_t* bp, uint64_t len) {
    _bp = bp;
    _fc = _len = len;
}
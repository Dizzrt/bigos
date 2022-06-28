#include "ktl\kbitset.h"

void kbitset::set(uint32_t pos) {
    uint8_t* bp = _bp + pos / 8;
    uint8_t mask = 0x80 >> (pos % 8);

    *bp |= mask;
    _fc--;
}

void kbitset::reset(uint32_t pos) {
    uint8_t* bp = _bp + pos / 8;
    uint8_t mask = 0x80 >> (pos % 8);

    *bp &= ~mask;
    _fc++;
}

void kbitset::set(uint32_t pos, uint32_t len) {
    uint8_t* bp = _bp + pos / 8;

    uint8_t _s = pos % 8;
    uint8_t mask = 0x80 >> _s;

    _s = 8 - _s;
    if (len <= _s)
        _s = len;
    len -= _s;

    while (_s--) {
        *bp |= mask;
        mask >>= 1;
    }
    bp++;

    _s = len / 8;
    while (_s--) {
        *bp = 0xff;
        bp++;
    }

    _fc -= len;
}

void kbitset::reset(uint32_t pos, uint32_t len) {
    uint8_t* bp = _bp + pos / 8;

    uint8_t _s = pos % 8;
    uint8_t mask = 0x80 >> _s;

    _s = 8 - _s;
    if (len <= _s)
        _s = len;
    len -= _s;

    while (_s--) {
        *bp &= ~mask;
        mask >>= 1;
    }
    bp++;

    _s = len / 8;
    while (_s--) {
        *bp = 0;
        bp++;
    }

    _fc += len;
}

inline bool kbitset::isFree(uint32_t pos) {
    return !(_bp[pos / 8] >> (8 - pos % 8 - 1) & 1);
}

uint32_t kbitset::scan(uint32_t len) {
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

inline void kbitset::__init__(uint8_t* bp, uint32_t len) {
    _bp = bp;
    _fc = _len = len;
}
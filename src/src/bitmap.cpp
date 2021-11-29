/*
 * @Author: Dizzrt
 * @Date: 2021-11-28 17:20:35
 * @LastEditTime: 2021-11-29 16:34:20
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\bitmap.cpp
 * @Description:
 */

#include "bitmap.h"

uint64_t bitmap_scan(BitMap *bitmap, uint64_t cnt) {
    uint64_t offset = -1;

    uint8_t *pointer = bitmap->bits;
    for (uint64_t _byte = 0; _byte < bitmap->len; _byte++) {
        if (*pointer == 0xff) {
            pointer++;
            continue;
        }

        uint64_t temp = 0;
        uint8_t mask = 0x80; // 1000 0000

        while (!(~(*pointer) & mask))
            mask >>= 1;

        // TODO bitmap scan
    }

    return -1;
}

void bitmap_init(BitMap *bitmap) { memset(bitmap->bits, 0xff, bitmap->len); }

// bit==1 => page available / bit==0 => page used
// isSet => make page used(set bit to 0)
void bitmap_update(BitMap *bitmap, uint64_t offset, uint64_t len, bool isSet) {

    uint8_t count, mask;
    uint8_t xoff = offset % 8;
    uint8_t *p = bitmap->bits;

    if (xoff != 0) {      // byte alignment
        count = 9 - xoff; // here is bits count
        if (len >= count)
            len -= count;
        else
            count = len, len = 0;

        p += offset / 8;

        count--;
        mask = BITMAP_MASK >> (xoff - 1);
        while (count--)
            mask = mask | (mask >> 1);

        if (isSet)
            *p = ~mask & *p;
        else
            *p = mask | *p;

        p++;
    }

    count = len / 8;  // here is byte count
    len -= count * 8; // the count of remaining bytes

    while (count--) {
        if (isSet)
            *p = 0;
        else
            *p = 0xff;
        p++;
    }

    len--;
    mask = BITMAP_MASK;
    while (len--)
        mask = mask | (mask >> 1);

    if (isSet)
        *p = ~mask & *p;
    else
        *p = mask | *p;

    return;
}
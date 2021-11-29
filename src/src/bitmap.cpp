/*
 * @Author: Dizzrt
 * @Date: 2021-11-28 17:20:35
 * @LastEditTime: 2021-11-29 18:33:55
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\bitmap.cpp
 * @Description:
 */

#include "bitmap.h"
#include "io.h"
uint64_t bitmap_scan(BitMap *bitmap, uint64_t cnt) {
    uint8_t *pointer = bitmap->bits;
    for (uint64_t p = 0; p < bitmap->len; p++) {
        // find the first byte which not 0x00
        if (*pointer == 0) {
            pointer++;
            continue;
        }

        // find the first bit which value is 1 in this byte
        uint8_t mask = BITMAP_MASK;
        do {
            if (*pointer & mask)
                break;
            mask >>= 1;
        } while (true);

        uint64_t __cnt = 0;
        do {
            if (*pointer & mask == mask)
                __cnt++;
            else
                break;

            if (__cnt == cnt) {
                uint64_t ret = (pointer - bitmap->bits) * 8;
                if (mask == 0x80)
                    ret += 0;
                else if (mask == 0x40)
                    ret += 1;
                else if (mask == 0x20)
                    ret += 2;
                else if (mask == 0x10)
                    ret += 3;
                else if (mask == 0x08)
                    ret += 4;
                else if (mask == 0x04)
                    ret += 5;
                else if (mask == 0x02)
                    ret += 6;
                else if (mask == 0x01)
                    ret += 7;

                return ret;
            }

            if (mask != 1)
                mask >>= 1;
            else {
                mask = BITMAP_MASK;
                pointer++;
                p++;
            }
        } while (true);

        pointer++;
    }

    return -1;
}

void bitmap_init(BitMap *bitmap) { memset(bitmap->bits, 0xff, bitmap->len / 8); }

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
            count = len + 1, len = 0;

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

    if (len > 0) {
        len--;
        mask = BITMAP_MASK;
        while (len--)
            mask = mask | (mask >> 1);

        if (isSet)
            *p = ~mask & *p;
        else
            *p = mask | *p;
    }

    return;
}
/*
 * @Author: Dizzrt
 * @Date: 2021-11-28 17:20:35
 * @LastEditTime: 2021-11-28 21:51:18
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\bitmap.cpp
 * @Description:
 */

#include "bitmap.h"

uint64_t bitmap_scan(BitMap *bitmap, uint64_t cnt) {
    uint64_t offset = -1;

    uint8_t *pointer = bitmap->bits;
    for (uint64_t byte = 0; byte < bitmap->len; byte++) {
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
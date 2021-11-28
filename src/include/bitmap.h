/*
 * @Author: Dizzrt
 * @Date: 2021-11-06 15:56:49
 * @LastEditTime: 2021-11-28 21:57:05
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\include\bitmap.h
 * @Description:
 */

#ifndef __BIG_BITMAP_H__
#define __BIG_BITMAP_H__
#include "stdint.h"

#define BITMAP_MASK 0x80
//#include "string.h"

struct BitMap {
    uint64_t len;
    uint8_t *bits;
};

extern "C" {
// void bitmap_init(bitmap *_bitmap);

uint64_t bitmap_scan(BitMap, uint64_t);
}

#endif
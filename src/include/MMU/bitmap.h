/*
 * @Author: Dizzrt
 * @Date: 2021-11-06 15:56:49
 * @LastEditTime: 2021-12-04 17:00:28
 * @LastEditors: Please set LastEditors
 * @FilePath: \bigos\src\include\bitmap.h
 * @Description:
 */

#ifndef __BIG_BITMAP_H__
#define __BIG_BITMAP_H__

#include "stdint.h"
#include "string.h"

#define BITMAP_MASK 0x80

struct BitMap {
    uint64_t len; // the count of bits
    uint8_t *bits;

    // bit==1 => free
    // bit==0 => in using
};

void bitmap_init(BitMap *);

// return the offset from bitmap.bits(step is bit)
uint64_t bitmap_scan(BitMap *, uint64_t);

// bitmap  offset  len  isSet
void bitmap_update(BitMap *, uint64_t, uint64_t, bool = true);

#endif
/*
 * @Author: Dizzrt
 * @Date: 2021-11-06 15:56:49
 * @LastEditTime: 2021-11-07 17:06:02
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\code\include\bitmap.h
 * @Description:
 */

#ifndef __BIG_BITMAP_H__
#define __BIG_BITMAP_H__
#include "stdint.h"

#define BITMAP_MASK 1
//#include "string.h"
struct bitmap {
    uint64_t size;
    uint8_t *bits;
};

extern "C" {
void bitmap_init(bitmap *_bitmap);
}

#endif
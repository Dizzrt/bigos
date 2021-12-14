/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-14 13:09:18
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\list.h"
#include "io.h"
#include "stdint.h"

struct Block {
    uint64_t addr;
};

extern list<Block *> blist[11];

// void *__alloc_pages(uint16_t);
// void pbuddy();

#endif
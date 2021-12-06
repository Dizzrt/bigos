/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 21:55:26
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "list.h"
#include "stdint.h"

struct Block {
    uint64_t addr;
};

list<Block> blist[11];

void *__alloc_pages(uint16_t);

#endif
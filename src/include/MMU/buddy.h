/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-11 18:32:00
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "io.h"
#include "list.h"
#include "stdint.h"

struct Block {
    uint64_t addr;
};

// extern list<Block *> blist[11];

void *__alloc_pages(uint16_t);
void pbuddy();
#endif
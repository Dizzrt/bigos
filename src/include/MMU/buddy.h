/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-15 17:48:56
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\list.h"
#include "io.h"
#include "stdint.h"

struct Block {
    uint8_t flags;

    uint64_t paddr; // physical address the first page
    uint64_t vaddr; // vitrual address

    uint32_t len; //
};

extern list<Block *> blist[11];

void *get_one_page();

#endif
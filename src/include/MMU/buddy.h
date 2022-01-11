/*
 * @Author: Dizzrt
 * @LastEditTime: 2022-01-02 20:06:04
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\list.h"
#include "KTL\map.h"
#include "slab.h"

static map<void*, uint32_t> __buddy_map;
static short __buddy_lens[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

struct __buddy_node {
    // uint8_t flags;
    uint64_t paddr;
};

extern int buddy_slab_objCnt;
extern Slab buddy_slab;
extern __list_node<Slab*> iBuddy_Slab_lnode;
extern Slab_cache buddy_node_cache;

extern list<__buddy_node*> blist[11];

void* __buddy_alloc(int = 1);

void __buddy_free(void*);
void __buddy_free(uint64_t, uint32_t);

__buddy_node* __get_buddy_node();

#endif
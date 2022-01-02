/*
 * @Author: Dizzrt
 * @LastEditTime: 2022-01-02 20:06:04
 */

#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\list.h"
#include "slab.h"
#include "stdint.h"

// struct Block {
//     uint8_t flags;

//     uint64_t paddr; // physical address the first page
//     uint64_t vaddr; // vitrual address

//     uint32_t len; //
// };

enum BUDDY_NODE_LEN { B1, B2, B4, B8, B16, B32, B64, B128, B256, B512, B1024 };

struct __buddy_node {
    uint8_t flags;
    uint64_t paddr;
};

extern int buddy_slab_objCnt;
extern Slab buddy_slab;
extern __list_node<Slab *> iBuddy_Slab_lnode;
extern Slab_cache buddy_node_cache;

extern list<__buddy_node *> blist[11];

void create_buddy_node(uint8_t, uint64_t, BUDDY_NODE_LEN);
void buddy_deg();

void *get_one_page();
void *get_pages(int);
#endif
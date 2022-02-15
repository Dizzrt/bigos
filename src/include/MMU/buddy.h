#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\klist.h"
#include "KTL\rb_tree.h"
#include "slab.h"
#include "stdint.h"

static short __buddy_lens[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

struct __buddy_node {
    // uint8_t flags;
    uint64_t paddr;
};

extern klist<__buddy_node> blist[11];

void* __buddy_alloc(uint32_t = 1);
void __buddy_free(void*);
void __buddy_free(uint64_t, uint32_t);

typedef _rb_tree_node<void*, uint32_t> buddyMapNode;

typedef linked_container<__buddy_node> buddy_container;
buddy_container* __alloc_buddy_container();

#endif
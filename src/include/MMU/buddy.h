#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "KTL\klist.h"
//#include "KTL\map.h"
#include "slab.h"
#include "stdint.h"

static short __buddy_lens[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

struct __buddy_node {
    // uint8_t flags;
    uint64_t paddr;
};

// extern list<__buddy_node*> blist[11];
extern klist<__buddy_node> blist[11];

void* __buddy_alloc(int = 1);
void __buddy_free(void*);
void __buddy_free(uint64_t, uint32_t);

typedef linked_container<__buddy_node> buddy_container;
buddy_container* __alloc_buddy_container();
void __free_buddy_container(buddy_container*);

#endif
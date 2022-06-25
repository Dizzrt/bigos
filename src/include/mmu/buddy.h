#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "memdef.h"
#include "ktl\klist.h"

#define GFP_DMA 0b01u
#define GFP_DMA32 0b10u
#define GFP_NORMAL 0b100u

extern uint64_t totalMemory;
extern uint64_t UsableMemory;
extern uint64_t usedMemory;

extern Zone Zone_DMA;
extern Zone Zone_DMA32;
extern Zone Zone_Normal;

MSeg* alloc_page(uint32_t gfp_flags = 0);
MSeg* alloc_pages(uint32_t gfp_flags = 0, uint32_t pages = 1);

void buddy_init();

#endif //__BIG_BUDDY_H__

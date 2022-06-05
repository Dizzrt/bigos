#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "memdef.h"
#include "ktl\klist.h"

#define GFP_DMA 0b01u
#define GFP_DMA32 0b10u
#define GFP_NORMAL 0b100u
#define GFP_

struct Zone
{
    uint64_t free_pages;
    uint64_t total_pages;

    klist<MSeg*> Segs[BUDDY_ORDER_CNT];
};

extern Zone Zone_DMA;
extern Zone Zone_DMA32;
extern Zone Zone_Normal;

// Pfs* alloc_pages(uint64_t);
// Pfs* alloc_page(); //alloc one page


MSeg* alloc_page(uint32_t gfp_flags);
MSeg* alloc_pages(uint32_t gfp_flags, uint32_t pages);

#endif //__BIG_BUDDY_H__

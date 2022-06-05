#include "mmu\buddy.h"

Zone Zone_DMA;
Zone Zone_DMA32;
Zone Zone_Normal;

enum TZone { NORMAL, DMA32, DMA };  //target Zone

static Zone* Zones[3] = { &Zone_Normal,&Zone_DMA32,&Zone_DMA };

MSeg* alloc_pages(uint32_t gfp_flags, uint32_t pages) {
    TZone tzone;
    int max_offset = 2;

    if (gfp_flags & GFP_DMA) {
        tzone = DMA;
        max_offset = 0;
    }
    else if (gfp_flags & GFP_DMA32) {
        tzone = DMA32;
        max_offset = 1;
    }
    else tzone = NORMAL;

    for (int i = 0;i <= max_offset;i++) {
        if (Zones[tzone]->free_pages < pages) {
            tzone = (TZone)(tzone + 1);
            continue;
        }



    }
}

MSeg* alloc_page(uint32_t gfp_flags) {


    MSeg* ret = nullptr;
    return ret;
}
#include "mmu\buddy.h"

Zone Zone_DMA;
Zone Zone_DMA32;
Zone Zone_Normal;

enum TZone { NORMAL, DMA32, DMA };  //target Zone

static Zone* Zones[3] = { &Zone_Normal,&Zone_DMA32,&Zone_DMA };

static uint32_t buddyChunkSize[BUDDY_ORDER_CNT] = {
    1,2,4,8,16,32,64,128,256,512,1024
};

MSeg* alloc_pages(uint32_t gfp_flags, uint32_t pages) {
    if (pages > BUDDY_MAX_PAGES) {
        //TODO give a hint 
        return nullptr;
    }

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

    while (max_offset
        && Zones[tzone]->free_pages < pages)
    {
        max_offset--;
        tzone = (TZone)(tzone + 1);
    }

    if (Zones[tzone]->free_pages < pages) {
        //TODO have no enough memory
    }

    MSeg* ret;
    Zone* _zone = Zones[tzone];

    int order = 0;
    while (_zone->Segs[order].empty() ||
        order <= BUDDY_MAX_ORDER && buddyChunkSize[order] < pages) {

    }

    int xpages = buddyChunkSize[order] - pages;
    // ret = _zone->Segs[order].front();
    klist<MSeg*>::iterator iter = _zone->Segs[order].begin();
    _zone->Segs[order].__list_remove(iter);

    ret = *iter;

    if (xpages) {
        //TODO 
    }

    return ret;
}

MSeg* alloc_page(uint32_t gfp_flags) {


    MSeg* ret = nullptr;
    return ret;
}
#include "mmu\buddy.h"
#include "new.h"

uint64_t totalMemory;
uint64_t UsableMemory;
uint64_t usedMemory;

Zone Zone_DMA;
Zone Zone_DMA32;
Zone Zone_Normal;

enum TZone { NORMAL, DMA32, DMA };  //target Zone

static Zone* Zones[3] = { &Zone_Normal,&Zone_DMA32,&Zone_DMA };

static uint32_t buddyChunkPages[BUDDY_ORDER_CNT] = {
    1,2,4,8,16,32,64,128,256,512,1024
};

static uint64_t buddyChunkSize[BUDDY_ORDER_CNT] = {
    1 * PAGE_SIZE,2 * PAGE_SIZE,4 * PAGE_SIZE,8 * PAGE_SIZE,16 * PAGE_SIZE,
    32 * PAGE_SIZE,64 * PAGE_SIZE,128 * PAGE_SIZE,256 * PAGE_SIZE,512 * PAGE_SIZE,1024 * PAGE_SIZE
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
        order <= BUDDY_MAX_ORDER && buddyChunkPages[order] < pages) {

    }

    int xpages = buddyChunkPages[order] - pages;
    // ret = _zone->Segs[order].front();
    klist<MSeg*>::iterator iter = _zone->Segs[order].begin();
    _zone->Segs[order].__list_remove(iter);

    ret = *iter;

    long long tbase = ret->base + 0x1000 * pages;
    if (xpages) {
        int t = BUDDY_MAX_ORDER;
        for (;t >= 0;t--) {
            while (xpages >= buddyChunkPages[t])
            {
                //TODO new chunck

                xpages -= buddyChunkPages[t];
                tbase += 0x1000 * buddyChunkPages[t];
            }
        }
    }

    return ret;
}

MSeg* alloc_page(uint32_t gfp_flags) {
    return alloc_pages(gfp_flags, 1);
}

static uint64_t initBuddyChunk(uint64_t base, uint64_t pages, Zone* zone) {
    for (uint32_t i = BUDDY_MAX_ORDER;i >= 0;i--) {
        while (pages >= buddyChunkPages[i])
        {
            MSeg* seg = nullptr;//TODO alloc
            new (seg) MSeg(base, buddyChunkPages[i], zone);

            linked_container<MSeg*>* lc = nullptr;//TODO alloc
            new (lc) linked_container<MSeg*>(seg);

            zone->Segs->__list_insert(lc);

            base += buddyChunkSize[i];
            pages -= buddyChunkPages[i];

            totalMemory += buddyChunkSize[i];
        }
    }

    return base;
}

static bool buddy_init_complete = false;
void buddy_init() {
    if (buddy_init_complete)
        return;

    uint32_t ARDSCount = *((uint32_t*)0x500);
    ARDS* ards_list = (ARDS*)0x580;

    uint64_t _base, _alignValue;
    uint64_t _pages, _endAddress;
    for (uint32_t i = 0;i < ARDSCount;i++) {
        //usable ram
        if (ards_list[i].type == 1) {
            _base = ards_list[i].base;
            _alignValue = _base % PAGE_SIZE;

            //align the base
            if (_alignValue) {
                _base = _base - _alignValue + PAGE_SIZE;
                _pages = (ards_list[i].length - _alignValue) / PAGE_SIZE;
            }
            else _pages = ards_list[i].length / PAGE_SIZE;

            _alignValue = _base + _pages * PAGE_SIZE;
            //ZONE_DMA
            if (_alignValue < ZONE_DMA_LIMIT)
                initBuddyChunk(_base, _pages, &Zone_DMA);
            //ZONE_DMA32
            else if (_alignValue < ZONE_DMA32_LIMIT) {
                if (_base < ZONE_DMA_LIMIT) {
                    uint64_t xpage = (ZONE_DMA_LIMIT - _base) / PAGE_SIZE;
                    _base = initBuddyChunk(_base, xpage, &Zone_DMA);
                    _pages -= xpage;
                }

                initBuddyChunk(_base, _pages, &Zone_DMA32);
            }
            //ZONE_NORMAL
            else {
                if (_base < ZONE_DMA32_LIMIT) {
                    uint64_t xpages;
                    if (_base < ZONE_DMA_LIMIT) {
                        xpages = (ZONE_DMA_LIMIT - _base) / PAGE_SIZE;
                        _base = initBuddyChunk(_base, xpages, &Zone_DMA);
                        _pages -= xpages;
                    }

                    xpages = (ZONE_DMA32_LIMIT - _base) / PAGE_SIZE;
                    _base = initBuddyChunk(_base, xpages, &Zone_DMA32);

                    _pages -= xpages;
                }

                initBuddyChunk(_base, _pages, &Zone_Normal);
            }

        }
    }


    uint32_t kSize = *((uint32_t*)0x504);

    //how many pages used by kernel
    kSize = kSize / PAGE_SIZE + (kSize % PAGE_SIZE == 0 ? 0 : 1);

    //TODO load kernel in high memory
    uint64_t kstart = 0x200000;
    uint64_t kend = kstart + kSize * PAGE_SIZE;

    //remove kernel used pages
    uint64_t xpages;
    klist<MSeg*>::iterator iter = Zone_DMA.Segs->begin();
    while (iter != Zone_DMA.Segs->end()) {
        MSeg* seg = *iter;
        uint64_t temp = seg->base + seg->pages * PAGE_SIZE;

        if (temp < kstart) {
            iter++;
            continue;
        }

        if (seg->base < kstart) {
            if (temp < kend) {
                xpages = (temp - kstart) / PAGE_SIZE;
                seg->pages -= xpages;
                kstart = temp;
                iter++;
            }
            else if (temp == kend) {
                seg->pages -= kSize;
                break;
            }
            else {
                seg->pages = (kstart - seg->base) / PAGE_SIZE;

                MSeg* _seg = nullptr;//TODO alloc
                linked_container<MSeg*>* lc = nullptr;//TODO alloc

                new (_seg) MSeg(kend, (temp - kend) / PAGE_SIZE, &Zone_DMA);
                new(lc) linked_container<MSeg*>(_seg);

                Zone_DMA.Segs->__list_insert(lc, iter);
                break;
            }
        }
        else if (seg->base == kstart) {
            if (temp < kend) {
                kstart = temp;
                iter++;

                Zone_DMA.Segs->__list_remove(seg);
                //TODO free
            }
            else if (temp == kend) {
                Zone_DMA.Segs->__list_remove(iter);
                //TODO free
                break;
            }
            else {
                seg->base = kend;
                seg->pages = (temp - kend) / PAGE_SIZE;

                break;
            }
        }
    }

}
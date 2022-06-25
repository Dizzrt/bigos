#ifndef __BIG_MEMDEF_H__
#define __BIG_MEMDEF_H__

#include "stdint.h"
#include "ktl\klist.h"

#define PAGE_SIZE 0x1000
#define BUDDY_MAX_ORDER 10
#define BUDDY_ORDER_CNT BUDDY_MAX_ORDER+1
#define BUDDY_MAX_PAGES 1<<BUDDY_MAX_ORDER

#define ZONE_DMA_LIMIT 0x1000000
#define ZONE_DMA32_LIMIT 0x100000000

//Address Range Descriptor Structure
struct ARDS
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    /*
     * Type 1: Usable (normal) RAM
     * Type 2: Reserved - unusable
     * Type 3: ACPI reclaimable memory
     * Type 4: ACPI NVS memory
     * Type 5: Area containing bad memory
    */

    uint32_t ACPI_exAttributes;
};

//physical memory segment
struct MSeg
{
    uint64_t base;
    uint64_t pages;

    void* Zone_ptr;

    MSeg(uint64_t _base, uint64_t _pages, void* ptr)
        : base(_base), pages(_pages), Zone_ptr(ptr) {}
};

//virtual memory segment
struct VMSeg
{
    uint64_t base;
    uint64_t pages;

    klist<MSeg*> pm_segs; //physical memory segments
};

struct Zone
{
    uint64_t free_pages;
    uint64_t total_pages;

    klist<MSeg*> Segs[BUDDY_ORDER_CNT];
};

#endif //__BIG_MEMDEF_H__
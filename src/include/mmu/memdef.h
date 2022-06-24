#ifndef __BIG_MEMDEF_H__
#define __BIG_MEMDEF_H__

#include "stdint.h"
#include "ktl\klist.h"

#define PAGE_SIZE 0x1000
#define BUDDY_MAX_ORDER 10
#define BUDDY_ORDER_CNT BUDDY_MAX_ORDER+1
#define BUDDY_MAX_PAGES 1<<BUDDY_MAX_ORDER

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
};

//virtual memory segment
struct VMSeg
{
    uint64_t base;
    uint64_t pages;

    klist<MSeg*> pm_segs; //physical memory segments
};




#endif //__BIG_MEMDEF_H__
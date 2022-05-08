#ifndef __BIG_MEMDEF_H__
#define __BIG_MEMDEF_H__

#include "stdint.h"
//#include "ktl\klist.h"

struct Pfs // page frames
{
    uint64_t base;
    uint32_t len;
    uint32_t flags;

    Pfs* next;

    Pfs() = default;
    Pfs(uint64_t _base = 0, uint32_t _len = 0, uint32_t _flags = 0) {}
};

struct MSeg
{
    //klist<Pfs*> pm_list; //physical memory
    Pfs* pm_list;


    uint64_t base;
    uint32_t len;
    uint32_t flags;
};




#endif //__BIG_MEMDEF_H__
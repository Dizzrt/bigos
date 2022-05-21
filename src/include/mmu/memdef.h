#ifndef __BIG_MEMDEF_H__
#define __BIG_MEMDEF_H__

#include "stdint.h"
//#include "ktl\klist.h"

struct Pfs // page frames
{
    uint64_t base;
    uint32_t len;
    uint32_t flags;

    Pfs* next = nullptr;

    Pfs() = default;
    Pfs(uint64_t _base = 0, uint32_t _len = 0, uint32_t _flags = 0) {}
};

struct PfsHeader
{
    uint32_t cnt;
    Pfs* pfs_list;
};


// struct MSeg
// {
//     //klist<Pfs*> pm_list; //physical memory
//     Pfs* pm_list;


//     uint64_t base;
//     uint32_t len;
//     uint32_t flags;
// };

struct VPfs // virtual page frames
{
    uint64_t base; // virtual address
    uint32_t len;

    uint32_t flags;

    PfsHeader* pfs;


    VPfs(uint64_t base, uint32_t len = 1, uint32_t flags = 0, PfsHeader* = nullptr);
};



#endif //__BIG_MEMDEF_H__
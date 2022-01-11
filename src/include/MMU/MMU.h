#ifndef __BIG_MMU_H__
#define __BIG_MMU_H__

#include "stdint.h"

struct AMS  // available memory segment
{
    uint64_t base;
    uint64_t len;
};

void memory_init();

#endif
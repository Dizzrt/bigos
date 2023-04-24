//
// File: memdef.h
// Created by Dizzrt on 2023/04/15.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_MEMDEF_H
#define _BIG_MEMDEF_H

#include <bigos/types.h>

#define SELF_MAPPING 0x100

#define PAGE_SIZE          0x1000ul
#define BUDDY_MAX_ORDER    10ul
#define CACHE_MAX_OBJ_SIZE 0x800ul

// GFM - get free memory

// alloc zone
#define _GFM_ZONE_NORMAL 0   // 00
#define _GFM_ZONE_DMA32  1   // 01
#define _GFM_ZONE_DMA    2   // 10
#define _GFM_ZONE_MASK   3   // 11

// cache's OBJ_SIZE must equal to SIZE
#define _GFM_PERFECT_FIT (1u << 2)
// if no cache is perfect fit, then create one
#define _GFM_NEW_CACHE_TO_PFIT (1u << 3)

#define _GFM_PRE_PAGING (1u << 4)

#define GFM_DEFAULT     0
#define GFM_PERFECT_FIT (_GFM_PERFECT_FIT | _GFM_NEW_CACHE_TO_PFIT)

#endif   // _BIG_MEMDEF_H

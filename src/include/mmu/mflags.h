#ifndef __BIG_MFLAGS_H__
#define __BIG_MFLAGS_H__

//ensure a slab will not be released
#define __SLAB_PERMANENT 0x00000001

#define SLAB_DEFAULT 0

//ensure there is at least one slab in the cache's empty list 
#define __CACHE_NONEMPTY 0x00000001

#define CACHE_DEFAULT 0

#define __GFP_DMA 0b01u
#define __GFP_DMA32 0b10u
#define __GFP_NORMAL 0b100u


#endif //__BIG_MFLAGS_H__
#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "memdef.h"

Pfs* alloc_pages(uint64_t);
Pfs* alloc_page(); //alloc one page

Pfs* valloc_pages(uint64_t);

void free_pages(Pfs*);


#endif //__BIG_BUDDY_H__

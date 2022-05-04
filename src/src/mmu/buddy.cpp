#include "mmu\buddy.h"
#include "ktl\klist.h"

typedef linked_container<Pfs*> pfs_containter;
typedef klist<Pfs*>::iterator iterator;

static klist<Pfs*> _buddy_list[11];

static uint64_t _buddy_size[] = { 1,2,4,8,16,32,64,128,256,512,1024 };
// static uint64_t _buddy_size[] = {
//     1 * 4096,2 * 4096,4 * 4096,8 * 4096,
//     16 * 4096,32 * 4096,64 * 4096,128 * 4096,
//     256 * 4096,512 * 4096,1024 * 4096
// };


Pfs* alloc_pages(uint64_t len) {
    //0x400000 => 4MB, 1024pt
    if (len > 0x400000)
        return nullptr; //TODO valloc_pages

    //how many pages need
    len = len / 4096 + (len % 4096 == 0 ? 0 : 1);

    int t = 10;
    while (len > _buddy_size[t])
        t--;

    iterator iter = _buddy_list[t].begin();
    _buddy_list->__list_remove(iter);
    Pfs* pfs = *iter;
    //TODO free container

    uint64_t _base = pfs->base + 0x1000 * len;

    t = 10;
    //how many pages were left
    len = _buddy_size[t] - len;
    while (len > _buddy_size[t])
    {
        //TODO new pfs_container
        pfs_containter* pfsc_temp = nullptr;
        Pfs* pfs_temp = nullptr;
        //TODO init
        _base += 0x1000 * _buddy_size[t];

        _buddy_list[t].__list_insert(pfsc_temp);
        len -= _buddy_size[t];
    }

    return pfs;
}

Pfs* alloc_page() { return alloc_pages(1); }

Pfs* valloc_pages(uint64_t len) {
    // Pfs* head = alloc_pages(len > 0x400000 ? 0x400000 : len);
    // Pfs* trail = head;

    // if (len > 0x400000) {
    //     len -= 0x400000;

    //     while (len > 0x400000)
    //     {
    //         trail->next = alloc_pages(0x400000);
    //         trail = trail->next;
    //         len -= 0x400000;
    //     }

    //     if (len > 0)
    //         trail->next = alloc_pages(len);
    // }

    // return head;
    //FIXME
}

void free_pages(Pfs* pfs) {
    do {
        //TODO free pfs

        pfs = pfs->next;
    } while (pfs != nullptr);
}
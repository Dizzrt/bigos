#include "mmu\mempool.h"
#include "mmu\buddy.h"
#include "new.h"

extern VMSeg* kmemAlloc_vmseg();
extern MSeg* kmemAlloc_mseg();
extern void* kmemAlloc_lcPonter();

void* MemPool::alloc(uint32_t pages, uint32_t gfp_flags) {
    void* ret = nullptr;
    //TODO alloc ret

    if (pages > maxPages_mapped) {
        // TODO alloc by buddy
    }
    else {
        // alloc by mapped_VMSeg

        VMSeg_iter iter = mapped_VMSeg.begin();

        VMSeg* seg = nullptr;
        while (iter != mapped_VMSeg.end())
        {
            if ((*iter)->pages >= pages) {
                seg = *iter;
                break;
            }
            else iter++;
        }

        ret = (void*)seg->base; // alloc address

        if (seg->pages == pages) {
            mapped_VMSeg.__list_remove(iter);

            if (used_VMSeg.empty())
                used_VMSeg.__list_insert(iter);
            else {
                for (VMSeg_iter i = used_VMSeg.begin();i != used_VMSeg.end();i++) {
                    if ((*i)->base > seg->base) {
                        used_VMSeg.__list_insert(iter, i);
                        break;
                    }
                }
            }
        }
        else {
            //TODO divide vmseg
            VMSeg* _vmseg = nullptr; //TODO alloc by slab

            _vmseg->base = seg->base + pages * 0x1000;
            _vmseg->pages = seg->pages - pages;
            // seg->pages = pages;

            int tpage = pages;

            //divide vmseg's pm_segs
            bool notHead = false;
            MSeg* mseg_t = nullptr;
            klist<MSeg*>::iterator iter = seg->pm_segs.begin();
            while (iter != seg->pm_segs.end())
            {
                mseg_t = *iter;
                if (tpage >= mseg_t->pages) {
                    tpage -= mseg_t->pages;
                }
                else {
                    if (tpage == 0)
                        _vmseg->pm_segs.__list_insert(iter);
                    else {
                        MSeg* _mseg = nullptr;//TODO alloc
                        linked_container<MSeg*>* _mseg_lc = nullptr;//TODO alloc

                        _mseg->Zone_ptr = mseg_t->Zone_ptr;
                        _mseg->pages = mseg_t->pages - tpage;
                        _mseg->base = mseg_t->base + tpage * 0x1000;

                        mseg_t->pages = tpage;
                        tpage = 0;

                        new (_mseg_lc) linked_container<MSeg*>(_mseg);
                        _vmseg->pm_segs.__list_insert(_mseg_lc);

                        notHead = true;
                    }
                }

                iter++;
            }

            //remove pm_segs from seg
            iter = _vmseg->pm_segs.begin();
            if (notHead)
                iter++;

            while (iter != _vmseg->pm_segs.end())
                seg->pm_segs.__list_remove(*iter);
        }
    }

    return ret;
}

//POOL_S_x => the start adress of x
//POOL_E_x => the end adress of x
//POOL_S_x => how many pages in x

#define POOL_S_NORMAL 0xffff880000000000
#define POOL_E_NORMAL 0xffffc80000000000
#define POOL_P_NORMAL \
    (POOL_E_NORMAL-POOL_S_NORMAL)/PAGE_SIZE

#define getlcv(VMSEGP,LCP) \
    LCP = (linked_container<VMSeg*>*)kmemAlloc_lcPonter(); \
    new (LCP) linked_container<VMSeg*> (VMSEGP)

extern MemPool kmempool;
void kmempool_init() {
    //unmapped vmem
    // VMSeg* _code = kmemAlloc_vmseg();
    // VMSeg* _pages = kmemAlloc_vmseg();
    // VMSeg* _vmalloc = kmemAlloc_vmseg();
    linked_container<VMSeg*>* _lc;

    VMSeg* _normal = kmemAlloc_vmseg();

    new (_normal) VMSeg(POOL_S_NORMAL, POOL_P_NORMAL, 0);
    getlcv(_normal, _lc);

    kmempool.unmapped_VMSeg.__list_insert(_lc);

}
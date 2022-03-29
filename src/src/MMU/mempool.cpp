#include "mmu\mempool.h"
#include "new.h"

mSeg::mSeg(uint64_t _base, uint64_t _len, uint8_t _flags)
    : base(_base), len(_len), flags(_flags) {}

void MemPool::_createSeg(uint64_t _base, uint64_t _len, uint8_t _flags) {
    mSeg* _mSeg;//TODO alloc seg
    new (_mSeg) mSeg(_base, _len, _flags);

    mSegNode* _mSegNode;//TODO alloc
    _mSegNode->val.second = _mSeg;

    freeSegs.insert(_mSegNode);
}

//TODO free seg

void* MemPool::_alloc(uint64_t pages) {
    mSeg* seg = nullptr;
    mSegNode* segNode = nullptr;
    uint64_t delta = UINT64_MAX;

    for (auto i : freeSegs) {
        if (i.second->len == pages) {
            delta = 0;
            seg = i.second;
            // segNode = 
            break;
        }
        else if (i.second->len < pages)
            continue;

        uint64_t temp = i.second->len - pages;
        if (temp < delta) {
            delta = temp;
            seg = i.second;
        }
    }



    if (delta > 0) {
        seg->len = pages;

    }

}
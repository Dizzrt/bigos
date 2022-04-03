#include "mmu\mempool.h"
#include "new.h"

mSeg::mSeg(uint64_t _base, uint64_t _len, uint8_t _flags)
    : base(_base), len(_len), flags(_flags) {}

void MemPool::_createSeg(uint64_t _base, uint64_t _len, uint8_t _flags) {
    mSeg* _mSeg = (mSeg*)getMSeg();
    new (_mSeg) mSeg(_base, _len, _flags);

    mSegNode* _mSegNode = (mSegNode*)getRbTree_8_8();
    new (_mSegNode) mSegNode(_base, _mSeg);
    _mSegNode->val.second = _mSeg;

    freeSegs.insert(_mSegNode);
}

void MemPool::_freeSeg(const void* p) {
    _rb_tree<uint64_t, mSeg*>::iterator iter = freeSegs.begin();

    while (iter != freeSegs.end())
    {
        if ((*iter).second->base != (uint64_t)p) {
            iter++;
            continue;
        }

        //TODO change pt and buddy    
        freeSegs.remove(iter.m_node);
        kmem_free(iter.m_node->val.second);
        kmem_free(iter.m_node);
        return;
    }
}

void* MemPool::_alloc(uint64_t pages) {
    mSeg* seg = nullptr;
    mSegNode* segNode = nullptr;
    uint64_t delta = UINT64_MAX;

    _rb_tree<uint64_t, mSeg*>::iterator iter = freeSegs.begin();
    while (iter != freeSegs.end())
    {
        if ((*iter).second->len < pages) {
            iter++;
            continue;
        }

        uint64_t temp = (*iter).second->len - pages;
        if (temp < delta) {
            segNode = iter.m_node;
            seg = iter.m_node->val.second;
            delta = temp;

            if (!delta)
                break;
        }
        //TODO need optimize
    }

    if (seg == nullptr) {
        //TODO param control
        return nullptr;
    }

    if (delta) {
        seg->len = pages;
        uint64_t tbase = seg->base + pages * 0x1000;

        mSeg* tseg = (mSeg*)getMSeg();
        mSegNode* tsegNode = (mSegNode*)getRbTree_8_8();

        new(tseg) mSeg(tbase, delta, seg->flags);
        new(tsegNode)mSegNode(tbase, tseg);

        freeSegs.insert(tsegNode);
    }

    freeSegs.remove(segNode);
    usedSegs.insert(segNode);

    return (void*)seg->base;
}

void MemPool::_free(const void* p) {
    uint64_t _p = (uint64_t)p;

    _rb_tree<uint64_t, mSeg*>::iterator iter = usedSegs.begin();
    while (iter != usedSegs.end())
    {
        if ((*iter).second->base == _p)
            break;
    }

    if (iter == usedSegs.end())
        return;

    usedSegs.remove(iter.m_node);
    freeSegs.insert(iter.m_node);
    //TODO merge
}

void MemPool::__merge(mSegNode* sn1, mSegNode* sn2) {

}
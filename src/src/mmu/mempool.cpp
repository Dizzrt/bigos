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

void* MemPool::_alloc(uint64_t pages, uint8_t _flags) {
    iterator iter = __search_len(pages, freeSegs);
    if (iter == freeSegs.end())
        return nullptr;//TODO judge _flags

    mSegNode* p = iter.m_node;
    mSeg* _mseg = p->val.second;
    uint64_t xlen = _mseg->len - pages;

    freeSegs.remove(p);
    usedSegs.insert(p);

    if (xlen) {
        _mseg->len -= xlen;
        _createSeg(_mseg->base + _mseg->len * 0x1000, xlen, _mseg->flags);
    }

    return (void*)_mseg->base;
}

void MemPool::_free(const void* p) {
    uint64_t _p = (uint64_t)p;

    _rb_tree<uint64_t, mSeg*>::iterator iter = usedSegs.begin();
    auto end = usedSegs.end();
    while (iter != end)
    {
        if ((*iter).second->base == _p)
            break;
    }

    //not a valid address
    if (iter == end)
        return;

    mSegNode* sn = iter.m_node;

    usedSegs.remove(sn);
    freeSegs.insert(sn);
    //TODO merge

    iter = iterator(sn);

}

_rb_tree_node<uint64_t, mSeg*>*
MemPool::__merge(mSegNode* sn1, mSegNode* sn2, _rb_tree<uint64_t, mSeg*>& segs) {
    mSeg* s1 = sn1->val.second;
    mSeg* s2 = sn2->val.second;

    segs.remove(sn1);
    segs.remove(sn2);

    mSeg* s = (mSeg*)getMSeg();
    s->base = s1->base < s2->base ? s1->base : s2->base;
    s->len = s1->len + s2->len;
    s->flags = s1->flags;

    mSegNode* sn = (mSegNode*)getRbTree_8_8();
    new (sn) mSegNode(s->base, s);

    kmem_free(s1);
    kmem_free(s2);
    kmem_free(sn1);
    kmem_free(sn2);

    segs.insert(sn);

    return sn;
}

_rb_tree<uint64_t, mSeg*>::iterator MemPool::__search_len(uint64_t _len, _rb_tree<uint64_t, mSeg*>& segs) {
    mSegNode* p = segs.__root().m_node;
    mSegNode* endp = segs.end().m_node;

    do {
        if (p->val.second->len == _len)
            return iterator(p);

        if (p->val.second->len > _len) {
            if (p->left != endp) {
                p = p->left;
                continue;
            }
            else return iterator(p);
        }

        if (p->val.second->len < _len) {
            if (p->right != endp) {
                p = p->right;
                continue;
            }
            else return ++iterator(p);
        }
    } while (p != endp);

    return segs.end();
}
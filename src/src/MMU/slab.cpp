/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-05 20:40:00
 */

#include "MMU\slab.h"
#include "io.h"
void *Slab_cache::__alloc(size_t len) {
    if (len > 0x1000)
        return nullptr;

    uint64_t offset;
    list<Slab *>::iterator iter = pool_partial.begin();
    Slab *t = *iter;
    // t = (Slab *)((uint64_t)t + 0x4000000000);
    printk_svga("0x%x\n", t);
    printk_svga("0x%x\n", &iter.node);
    return (void *)(long long)0;

    while (iter != pool_partial.end()) {

        if ((*iter)->__free >= len)
            goto flag;

        offset = bitmap_scan(&(*iter)->bitmap, len);

        if (offset != -1)
            return (void *)((long long)offset);

    flag:
        iter++;
    }

    return nullptr;
}

void Slab_cache::appendSlab(Slab *slab) {}

void Slab_cache::__appendSlab_(__list_node<Slab *> *node, PoolType _type) {
    switch (_type) {
        case PoolType::FULL: pool_full.__push_back_(node); break;
        case PoolType::EMPTY: pool_empty.__push_back_(node); break;
        case PoolType::PARTIAL: pool_partial.__push_back_(node); break;
        default: break;
    }
}
#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "ktl\klist.h"
#include "ktl\rb_tree.h"

struct buddyBlock
{
    uint8_t flags;
    uint32_t len;
};

#define RB_BUDDY_SIZE sizeof(_rb_tree_node<void*, buddyBlock>)
typedef _rb_tree_node<void*, buddyBlock> _rb_buddyBlock;
extern _rb_buddyBlock* getRB_buddyBlock();

void* buddy_alloc(unsigned int = 1);
void buddy_free(const void*);

void buddyInit();

#endif //__BIG_BUDDY_H__
#ifndef __BIG_BUDDY_H__
#define __BIG_BUDDY_H__

#include "ktl\klist.h"
#include "ktl\rb_tree.h"

//#define RB_BUDDY_SIZE sizeof(_rb_tree_node<void*, buddyBlock>)
//typedef _rb_tree_node<void*, buddyBlock> _rb_buddyBlock;
//extern _rb_buddyBlock* getRB_buddyBlock();
extern linked_container<void*>* getLC_8B();
extern void freeLC_8B(const void*);

void* buddy_alloc(unsigned int);
void buddy_free(uint64_t, uint64_t);

void buddyInit();

#endif //__BIG_BUDDY_H__
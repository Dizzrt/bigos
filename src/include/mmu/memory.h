#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

void* kmalloc(unsigned int size);
void kfree(const void* p);

#endif //__BIG_MEMORY_H__
/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-29 17:49:21
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__
#include "bitmap.h"
#include "stdint.h"

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

struct AMS // available memory segment
{
    uint64_t base;
    uint64_t len;
};

struct MemoryInfo {
    uint64_t size;
    uint64_t free;
};
extern MemoryInfo memInfo;

void memory_init();
void *kmalloc(uint64_t);
void kfree(const void *);

#endif
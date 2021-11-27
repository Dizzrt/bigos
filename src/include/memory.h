/*
 * @Author: Dizzrt
 * @Date: 2021-11-27 16:59:38
 * @LastEditTime: 2021-11-27 17:04:18
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\include\memory.h
 * @Description:
 */

#ifndef __BIG_MEMORY_H__
#define __BIG_MEMORY_H__

#include "stdint.h"

struct AMS // available memory segment
{
    uint64_t base;
    uint64_t len;
};

static uint64_t totalMemory;

extern "C" {
void test();
}

#endif
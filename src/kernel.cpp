/*
 * @Author: Dizzrt
 * @Date: 2021-10-10 21:42:47
 * @LastEditTime: 2021-11-28 16:45:19
 * @LastEditors: Dizzrt
 * @Description:
 * @FilePath: \bigos\src\kernel.cpp
 */

#include "dev/svga.h"
#include "dev/timer.h"
#include "global.h"
#include "interrupt.h"
#include "io.h"
#include "memory.h"

extern "C" {
void Kernel();
static void init_kernel();
}

void Kernel() {
    init_kernel();

    svga_Clear();
    svga_SetCursorPos(0, 2);

    char msg_kernel[] = "in kernel";
    char *addr = (char *)0x40000b8000;
    asm volatile("nop\nnop");
    uint32_t *amsCount = (uint32_t *)0x504;

    //__put_int__(*amsCount, INT_MODE::DEC);

    for (int i = 0; i < 9; i++) {
        *addr++ = msg_kernel[i];
        *addr++ = 0x0c;
    }

    test();

    while (1) {};
}

static void init_kernel() {
    init_memory();

    ppt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
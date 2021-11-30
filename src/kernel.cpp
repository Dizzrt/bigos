/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-11-30 15:40:17
 */

#include "dev/svga.h"
#include "dev/timer.h"
#include "global.h"
#include "interrupt.h"
#include "io.h"
#include "memory.h"

extern "C" void Kernel();
static void init_kernel();

void Kernel() {
    init_kernel();

    // svga_Clear();
    // svga_SetCursorPos(0, 2);

    // {
    //     char msg_kernel[] = "in kernel";
    //     char *addr = (char *)0x40000b8000;

    //     for (int i = 0; i < 9; i++) {
    //         *addr++ = msg_kernel[i];
    //         *addr++ = 0x0c;
    //     }
    // }

    while (true)
        asm volatile("hlt");
}

static void init_kernel() {
    memory_init();

    ppt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
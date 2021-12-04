/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-04 20:53:54
 */

#include "MMU/memory.h"
#include "MMU/slab.h"
#include "dev/svga.h"
#include "dev/timer.h"
#include "global.h"
#include "interrupt.h"
#include "io.h"

#include "list.h"

extern "C" void Kernel();
static void init_kernel();

void Kernel() {

    init_kernel();
    svga_Clear();

    printk_svga("big OS(developing)\n", 123);

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    ppt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
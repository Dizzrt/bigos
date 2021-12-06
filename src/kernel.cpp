/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 20:55:44
 */

#include "MMU/memory.h"
#include "MMU/slab.h"
#include "dev/svga.h"
#include "dev/timer.h"
#include "interrupt.h"
#include "io.h"

#include "list.h"

extern "C" void Kernel();
static void init_kernel();

void Kernel() {
    svga_Clear();
    init_kernel();
    svga_SetCursorPos(0, 1);

    printk_svga("addr: 0x%x", (uint64_t)kmalloc(10));

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
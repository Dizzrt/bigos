/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-31 19:40:33
 */

//#include "MMU\buddy.h"
#include "MMU\memory.h"
#include "MMU\slab.h"
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "io.h"

extern "C" void Kernel();
static void init_kernel();

void Kernel() {
    svga_Clear();
    svga_SetCursorPos(0, 1);
    init_kernel();

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}

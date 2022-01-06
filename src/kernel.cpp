/*
 * @Author: Dizzrt
 * @LastEditTime: 2022-01-03 11:46:52
 */

#include "KTL\map.h"
#include "MMU\memory.h"
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

    map<int, int> mp;
    mp[0] = 2;
    int temp = mp.find(0);
    printk_svga("%d\n", temp);

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);  // 1193=1193180/中断频率(100)
    intr_init();
}

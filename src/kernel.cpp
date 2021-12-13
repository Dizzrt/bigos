/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-13 22:16:52
 */

//#include "MMU/buddy.h"
#include "MMU/memory.h"
#include "MMU/slab.h"
#include "dev/svga.h"
#include "dev/timer.h"
#include "interrupt.h"
#include "io.h"
#include "list.h"

extern "C" void Kernel();
static void init_kernel();

list<int> test;
void Kernel() {
    init_kernel();
    svga_Clear();
    svga_SetCursorPos(0, 1);

    test.push_back(1);
    test.push_back(2);

    test.push_back(3);
    test.push_back(4);
    test.push_front(0);

    list<int>::iterator iter = test.begin();
    do
        printk_svga("%d\n", *iter);
    while (++iter != test.end());

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}

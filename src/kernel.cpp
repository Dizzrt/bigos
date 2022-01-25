//#include "KTL\map.h"
//#include "MMU\MMU.h"
#include "KTL\list.h"
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "io.h"

void Kernel() {
    svga_Clear();
    svga_SetCursorPos(0, 1);
    init_kernel();

    list<int> a;
    list<int>::iterator iter = a.begin();

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);  // 1193=1193180/中断频率(100)
    intr_init();
}

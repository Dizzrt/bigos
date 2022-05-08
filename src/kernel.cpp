
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "io.h"

// #include "mmu\kmem.h"
// #include "ktl\kbitset.h"

extern "C" void Kernel();
static void init_kernel();

void Kernel() {
    init_kernel();
    //----------------//

    //----------------//
    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    svga_Clear();
    svga_SetCursorPos(0, 0);
    // kmemInit();


     // 1193=1193180/中断频率(100)
    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);
    intr_init();
}

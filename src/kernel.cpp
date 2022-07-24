
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "mmu\memory.h"
#include "io.h"

extern void kmem_init();
extern "C" void Kernel();
static void init_kernel();

#include "ktl\list.h"

struct tst {
    int x, y;
    tst(int _x) { x = _x, y = _x * _x; }
    ~tst() { printk_svga("destory tst\n"); }
};

void Kernel() {
    init_kernel();
    //----------------//

    kstd::list<int> ls;


    //----------------//
    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    svga_Clear();
    kmem_init();

    // 1193=1193180/中断频率(100)
    // pt == programmable timer
    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);
    intr_init();
}

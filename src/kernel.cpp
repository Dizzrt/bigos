
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "io.h"

// #include "mmu\kmem.h"
// #include "ktl\kbitset.h"

#include "ktl\klist.h"
#include "new.h"
extern "C" void Kernel();
static void init_kernel();

klist<int> t;

linked_container<int> t1(1);
linked_container<int> t2(2);
linked_container<int> t3(3);

void Kernel() {
    init_kernel();
    //----------------//

    t.__list_insert(&t1);
    t.__list_insert(&t3);

    t.__list_insert(&t2, t.begin());

    for (auto i : t)
        printk_svga("%d\n", i);


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


#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "mmu\memory.h"
#include "io.h"

extern void kmem_init();
extern "C" void Kernel();
static void init_kernel();

void Kernel() {
    init_kernel();
    //----------------//

    printk_svga("welcom bigOS\n");
    uint32_t kSize = *((uint32_t*)0x504);
    printk_svga("kernel size:%d\n", kSize);

    int* p = (int*)kmalloc(sizeof(int) * 10);
    for (int i = 0;i < 10;i++)
        p[i] = i * 2;
    for (int i = 0;i < 10;i++)
        printk_svga("%d\n", p[i]);

    // klist<int> kt;
    // linked_container<int>lc1(1);
    // linked_container<int>lc2(2);

    // kt.__list_insert(&lc1);
    // kt.__list_insert(&lc2, kt.end());

    // for (auto i : kt)
    //     printk_svga("%d\n", i);


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

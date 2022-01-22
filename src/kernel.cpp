//#include "KTL\map.h"
//#include "MMU\MMU.h"
#include "dev\svga.h"
#include "dev\timer.h"
#include "interrupt.h"
#include "io.h"
#include "new.h"

extern "C" void Kernel();
static void init_kernel();

class test {
  private:
    /* data */
  public:
    int t;
    test(/* args */) { t = 1; }
    test(int tt) { t = tt; }
    ~test() { printk_svga("destructor!\n"); };
};

void Kernel() {
    svga_Clear();
    svga_SetCursorPos(0, 1);
    init_kernel();

    test a;
    test* r = new (&a) test(123);
    printk_svga("%d\n", a.t);
    r = new (&a) test();
    printk_svga("%d\n", a.t);

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);  // 1193=1193180/中断频率(100)
    intr_init();
}

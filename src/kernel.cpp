//#include "KTL\map.h"
//#include "MMU\MMU.h"

#include "KTL\klist.h"
#include "KTL\rb_tree.h"
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
#pragma region rb tree test
// _rb_tree<int, int> t;
// _rb_tree_node<int, int> t1(5, 1);
// _rb_tree_node<int, int> t2(3, 2);
// _rb_tree_node<int, int> t3(8, 3);
// _rb_tree_node<int, int> t4(7, 4);
// _rb_tree_node<int, int> t5(6, 4);
// _rb_tree_node<int, int> t6(4, 4);
// _rb_tree_node<int, int> t7(2, 4);
// _rb_tree_node<int, int> t8(1, 4);
// _rb_tree_node<int, int> t9(0, 4);
// _rb_tree_node<int, int> t10(11, 4);
// printk_svga("ep:%d  count:%d\n", t.empty(), t.size());
// t.__insert(&t1);
// printk_svga("%d\n", t1.isRed);
// t.__insert(&t2);
// printk_svga("%d\t%d\n", t1.isRed, t2.isRed);
// t.__insert(&t3);
// printk_svga("%d\t%d\t%d\n", t1.isRed, t2.isRed, t3.isRed);
// t.__insert(&t4);
// printk_svga("%d\t%d\t%d\t%d\n", t1.isRed, t2.isRed, t3.isRed, t4.isRed);
// t.__insert(&t5);
// printk_svga("%d\t%d\t%d\t%d\t%d\n", t1.isRed, t2.isRed, t3.isRed, t4.isRed, t5.isRed);

// t.__insert(&t6);
// t.__insert(&t7);
// t.__insert(&t8);
// t.__insert(&t9);
// t.__insert(&t10);
// printk_svga("%d %d %d %d %d %d %d %d %d %d\n", t1.isRed, t2.isRed, t3.isRed, t4.isRed, t5.isRed, t6.isRed, t7.isRed,
//     t8.isRed, t9.isRed, t10.isRed);

// printk_svga("ep:%d  count:%d\n", t.empty(), t.size());
#pragma endregion

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ);  // 1193=1193180/中断频率(100)
    intr_init();
}

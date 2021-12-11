/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-11 20:15:43
 */

// #include "MMU/buddy.h"
// #include "MMU/memory.h"
// #include "MMU/slab.h"
#include "dev/svga.h"
#include "dev/timer.h"
#include "interrupt.h"
#include "io.h"

#include "list.h"
#define size_t long long

extern "C" void Kernel();
static void init_kernel();

struct test {
    int val;
    list_node list;
};
test t1;
test t2;
void Kernel() {
    svga_Clear();
    init_kernel();
    svga_SetCursorPos(0, 1);

    t1.val = 1;
    t2.val = 2;

    list_node *head = &t1.list;

    list_add(&t2.list, &t1.list);

    list_iterator<test> iter = &t1.list;
    list_iterator<test> iter2 = iter;
    do {
        printk_svga("%d\n", (*iter)->val);
        iter++;
    } while (iter2 != iter);

    // test *t = container_of(&t1.list, test, list);
    // printk_svga("%d\n", t->val);

    // t = container_of(t1.list.next, test, list);

    // printk_svga("%d\n", t->val);

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    pt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
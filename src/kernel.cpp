/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-05 22:12:15
 */

#include "MMU/memory.h"
#include "MMU/slab.h"
#include "dev/svga.h"
#include "dev/timer.h"
#include "global.h"
#include "interrupt.h"
#include "io.h"

#include "list.h"

extern "C" void Kernel();
static void init_kernel();
Slab tslab;
list<Slab *> tlist;
__list_node<Slab *> tnode;
void Kernel() {
    // svga_Clear();
    init_kernel();
    svga_SetCursorPos(0, 1);

    tlist.node.next = &tlist.node;
    tlist.node.prev = &tlist.node; // MARKER 不知道为何定义全局变量的类对象不会自动调用构造函数。

    tslab.__free = 123;
    tnode.val = &tslab;
    printk_svga("0x%x\n", &tlist);
    tlist.__push_back_(&tnode);

    __list_node<Slab *> *tmp = tlist.node.next;
    printk_svga("this cnt is %d\n", tlist.__size);
    printk_svga("0x%x\n", &tnode);
    printk_svga("tmp is 0x%x\n", tmp);

    printk_svga("val is 0x%x\n", tmp->val);
    //  list<Slab *>::iterator iter = tlist.begin();
    //  printk_svga("%d\n", (*iter)->__free);

    while (true) {
        asm volatile("hlt");
    }
}

static void init_kernel() {
    // memory_init();

    ppt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
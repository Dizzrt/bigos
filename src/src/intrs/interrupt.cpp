/*
 * @Author: Dizzrt
 * @Date: 2021-11-01 12:00:41
 * @LastEditTime: 2021-11-24 21:10:32
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\intrs\interrupt.cpp
 * @Description:
 */
#include "interrupt.h"

#include "io.h"

void intr_init() {
    // idt_init();
    // init idt
    for (int i = 0; i < 64; i++)
        intr_register_handler(i, intr_entry_table[i]);

    pic_init(0xfc, 0xff); //初始化中断代理芯片
    intr_Set(intr_status::INTR_ON);
}

// void idt_init() {
//     intr_register_handler(0x20, intr_timer);    // IRQ0----时钟中断
//     intr_register_handler(0x21, intr_keyboard); // IRQ1----键盘中断
// }

void pic_init(uint8_t OCW1_m, uint8_t OCW1_s) // for 8259A
{
    //主片
    __outb__(0x20, 0x11);   // ICW1----级联、边沿触发
    __outb__(0x21, 0x20);   // ICW2----起始中断号
    __outb__(0x21, 0x04);   // ICW3----IR2接从片
    __outb__(0x21, 0x01);   // ICW4----
    __outb__(0x21, OCW1_m); // OCW1----中断屏蔽字

    //从片
    __outb__(0xa0, 0x11);
    __outb__(0xa1, 0x28);
    __outb__(0xa1, 0x02);
    __outb__(0xa1, 0x01);
    __outb__(0xa1, OCW1_s);
}

void intr_register_handler(uint8_t vnum, intr_handler handler) {
    uint64_t *p = 0;
    p += vnum * 2;

    uint64_t addr_low = (uint64_t)handler & 0x000000000000ffff;
    uint64_t addr_mid = (uint64_t)handler & 0x00000000ffff0000;
    uint64_t addr_high = (uint64_t)handler & 0xffffffff00000000;

    addr_mid <<= 32;
    addr_high >>= 32;

    uint64_t temp = addr_low | addr_mid | 0x00008e0000180000;

    *p = temp;
    *(p + 1) = addr_high;

    return;
}

intr_status intr_Get() {
    uint64_t flags;
    asm volatile("pushf\n"
                 "pop %0"
                 : "=g"(flags));

    flags &= 0x200;
    if (flags == 0x200)
        return intr_status::INTR_ON;
    return intr_status::INTR_OFF;
}

intr_status intr_Set(intr_status status) {
    if (status == intr_status::INTR_OFF)
        asm volatile("cli");

    if (status == intr_status::INTR_ON)
        asm volatile("sti");

    // return intr_Get();
    return intr_status::INTR_ON;
}

void do_intr(uint64_t ivec, uint64_t ecode) {}
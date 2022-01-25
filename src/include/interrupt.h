#ifndef __BIG_INTERRUPT_H__
#define __BIG_INTERRUPT_H__

#include "stdint.h"
extern void* intr_entry_table[48];
typedef void (*intr_handler)(uint64_t);
static intr_handler intr_handler_table[48];

enum intr_status { INTR_OFF, INTR_ON };

extern "C" {
// void intr_timer();
void intr_timer(uint64_t);
void intr_keyboard(uint64_t);
}

extern "C" {
void reg_intrs();
void do_intr(uint8_t, uint64_t);  // TODO optimize intr

/**
 * @description: 初始化中断代理芯片(8259A)
 * @param {uint8_t} 主片OCW1
 * @param {uint8_t} 从片OCW1
 */
void pic_init(uint8_t, uint8_t);  // programable interrupt controller

void intr_init();

intr_status intr_Get();
intr_status intr_Set(intr_status);

/**
 * @description: 注册一个中断服务程序
 * @param {uint_8} 中断号
 * @param {intr_handler} 中断服务程序
 */
void intr_register_handler(uint8_t, intr_handler);
static void inline intr_register_entry(uint8_t, void*);
}

#endif
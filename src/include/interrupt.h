/*
 * @Author: Dizzrt
 * @Date: 2021-11-01 11:05:33
 * @LastEditTime: 2021-11-24 21:05:04
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\include\interrupt.h
 * @Description: 所有中断服务在此进行注册，中断服务程序应在此文件中声明,最多64个中断服务，最大idt号0x40
 */
#ifndef __BIG_INTERRUPT_H__
#define __BIG_INTERRUPT_H__

#define iret __iret__()

#include "stdint.h"
typedef void (*intr_handler)();
extern intr_handler intr_entry_table[64];
enum intr_status { INTR_OFF, INTR_ON };

extern "C" {
// void intr_timer();
void intr_timer();
void intr_keyboard();
}

extern "C" {
void do_intr(uint64_t, uint64_t);

//初始化中断门描述符
void idt_init();

/**
 * @description: 初始化中断代理芯片(8259A)
 * @param {uint8_t} 主片OCW1
 * @param {uint8_t} 从片OCW1
 */
void pic_init(uint8_t, uint8_t); // programable interrupt controller

void intr_init();

intr_status intr_Get();
intr_status intr_Set(intr_status);

/**
 * @description: 注册一个中断服务程序
 * @param {uint_8} 中断号
 * @param {intr_handler} 中断服务程序
 */
void intr_register_handler(uint8_t, void (*)());
}

#endif
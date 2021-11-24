/*
 * @Author: Dizzrt
 * @Date: 2021-11-01 18:24:28
 * @LastEditTime: 2021-11-21 14:09:38
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\src\src\intrs\intr_timer.cpp
 * @Description:
 */

#include "interrupt.h"
#include "io.h"
unsigned char i = '0';
extern "C" void intr_timer() {
    // intr_Set(intr_status::INTR_OFF);

    char *p = (char *)0xb80a2;
    *p = i++;
    *(p + 1) = 0x0c;

    if (i > '9')
        i = '0';

    // intr_Set(intr_status::INTR_ON);
    // outb(0x20, 0x20);
    // outb(0xa0, 0x20);

    iret;
}
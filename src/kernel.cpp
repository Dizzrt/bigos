/*
 * @Author: Dizzrt
 * @Date: 2021-10-10 21:42:47
 * @LastEditTime: 2021-11-23 15:49:38
 * @LastEditors: Dizzrt
 * @Description:
 * @FilePath: \Big OS\src\kernel.cpp
 */

#include "dev/svga.h"
#include "dev/timer.h"
#include "global.h"
#include "interrupt.h"
#include "io.h"
//#include "string.h"

extern "C" {
void Kernel();
static void init_kernel();
}

void Kernel() {
    init_kernel();
    svga_Clear();

    char msg_kernel[] = "in kernel";
    char *addr = (char *)0x40000b8000;

    for (int i = 0; i < 9; i++) {
        *addr++ = msg_kernel[i];
        *addr++ = 0x0c;
    }

    // memset((void *)0x4000000000, 0xff, 1024);

    // svga_SetCursorPos(0, 2);
    //  uint8_t *base = (uint8_t *)0x600;
    //  uint8_t *lenth = (uint8_t *)0x608;
    //  uint8_t *flag = (uint8_t *)0x610;

    // while (true) {
    //     if (*((uint32_t *)flag) == 0)
    //         break;

    //     __put_int__(*((uint64_t *)base), INT_MODE::HEX);
    //     __put_char__('\t');
    //     __put_int__(*((uint64_t *)lenth), INT_MODE::HEX);
    //     __put_char__('\t');
    //     __put_int__(*((uint32_t *)flag), INT_MODE::HEX);
    //     __put_char__('\n');

    //     base += 20;
    //     lenth += 20;
    //     flag += 20;
    // }
    // char str[] = "123456789123456789";
    // __put_int__(strlen(str), INT_MODE::DEC);

    while (1) {};
}

static void init_kernel() {
    ppt_init(0x34, (CLOCK_TICK_RATE + HZ / 2) / HZ); // 1193=1193180/中断频率(100)
    intr_init();
}
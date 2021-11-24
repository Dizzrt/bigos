/*
 * @Author: Dizzrt
 * @Date: 2021-11-03 17:35:01
 * @LastEditTime: 2021-11-19 13:12:40
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\src\include\io.h
 * @Description:
 */

#ifndef __BIG_IO_H__
#define __BIG_IO_H__
#include "stdint.h"
// output for text mode
enum INT_MODE { HEX, DEC, OCT, BIN };

extern "C" {
uint8_t __inb__(uint16_t);
void __outb__(uint16_t, uint8_t);

void __put_char__(uint8_t);
void __put_int__(uint64_t, INT_MODE);
}

#endif
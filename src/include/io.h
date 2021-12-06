/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-06 21:01:40
 */

#ifndef __BIG_IO_H__
#define __BIG_IO_H__

#include "stdarg.h"
#include "stdint.h"

#define TAB_WIDTH 4

// output for text mode
enum INT_MODE { HEX, DEC, OCT, BIN };

uint8_t __inb__(uint16_t);
void __outb__(uint16_t, uint8_t);

void putk_svag(char);
void putsk_svga(const char *);
void printk_svga(const char *, ...);

#endif
/*
 * @Author: Dizzrt
 * @Date: 2021-11-03 17:35:01
 * @LastEditTime: 2021-12-02 16:08:47
 * @LastEditors: Please set LastEditors
 * @FilePath: \Big OS\src\include\io.h
 * @Description:
 */

#ifndef __BIG_IO_H__
#define __BIG_IO_H__

#include "stdarg.h"
#include "stdint.h"
#include "string.h"

#define TAB_WIDTH 4

// output for text mode
enum INT_MODE { HEX, DEC, OCT, BIN };

uint8_t __inb__(uint16_t);
void __outb__(uint16_t, uint8_t);

void putk_svag(char);
void putsk_svga(const char *);
void printk_svga(const char *, ...);

#endif
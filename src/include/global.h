/*
 * @Author: Dizzrt
 * @Date: 2021-10-31 16:44:24
 * @LastEditTime: 2021-11-04 13:32:13
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\Source\cpp\include\sys\global.h
 * @Description: 各种全局变量在此文件中定义
 */
#ifndef __BIG_GLOBAL_H__
#define __BIG_GLOBAL_H__

#include "stdint.h"

static char kernelInfo[] = "Big OS v0.1";

#define SELECTOR_CODE 0x18
#define SELECTOR_DATA 0x20
#define SELECTOR_STACK 0x28
#define SELECTOR_VIDEO 0x30

#define CLOCK_TICK_RATE 1193180
#define HZ 100

static uint16_t resolution_x = 80;
static uint16_t resolution_y = 25;

#endif
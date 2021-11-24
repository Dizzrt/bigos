/*
 * @Author: Dizzrt
 * @Date: 2021-11-02 19:47:29
 * @LastEditTime: 2021-11-02 20:24:56
 * @LastEditors: Dizzrt
 * @FilePath: \Big OS\Source\cpp\include\dev\driver\timer.h
 * @Description:
 */

#ifndef __BIG_TIMER_H__
#define __BIG_TIMER_H__
#include "stdint.h"
extern "C" {
/**
 * @description: initalize the programable interrupt timer
 * @param {uint8_t} control word
 * @param {uint16_t} count value
 */
void ppt_init(uint8_t, uint16_t);

/**
 * @description: set the counter's start value
 * @param {uint8_t} select counter
 * @param {uint16_t} count value
 */
void set_ppt_value(uint8_t, uint8_t, uint16_t);
}

#endif

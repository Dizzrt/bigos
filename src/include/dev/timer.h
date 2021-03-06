#ifndef __BIG_TIMER_H__
#define __BIG_TIMER_H__

// 8253 impulse frequency
#define CLOCK_TICK_RATE 1193180
// clock interrupt rate
#define HZ 100

#include "stdint.h"

// pt -- programmable timer

/**
 * @description: initalize the programable interrupt timer
 * @param {uint8_t} control word
 * @param {uint16_t} count value
 */
void pt_init(uint8_t, uint16_t);

/**
 * @description: set the counter's start value
 * @param {uint8_t} select counter
 * @param {uint16_t} count value
 */
void set_pt_value(uint8_t, uint8_t, uint16_t);

#endif

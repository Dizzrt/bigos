#ifndef __BIG_SVGA_H__
#define __BIG_SVGA_H__

#include "stdint.h"

extern uint16_t resolution_x;
extern uint16_t resolution_y;

/**
 * @description: 设置显示模式
 * @param {uint-8} 显示模式
 */
void svga_SetMode(uint8_t);
uint16_t svga_GetCursorPos();

/**
 * @description: set the cursor postion
 * @param {uint_16} cursor position x
 * @param {uint_16} cursor position y
 * @return {*}
 */
void svga_SetCursorPos(uint16_t, uint16_t);
void __svga_SetCursorPos__(uint16_t);
/**
 * @description: move the cursor (offset units) from the current postion
 * @param {int16_t} offset
 * @param {bool} if true,the cursor move forward else move backward
 */
void svga_MoveCursor(uint16_t, bool);
void svga_Clear();

#endif
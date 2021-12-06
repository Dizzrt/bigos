/*
 * @Author: Dizzrt
 * @Date: 2021-11-03 10:59:30
 * @LastEditTime: 2021-12-06 21:02:47
 * @LastEditors: Please set LastEditors
 * @FilePath: \Big OS\src\src\dev\svga.cpp
 * @Description: svga driver
 */

#include "dev/svga.h"
#include "io.h"

uint16_t resolution_x = 80;
uint16_t resolution_y = 25;

void svga_SetMode(uint8_t) {} // TODO 设置显示模式
uint16_t svga_GetCursorPos() {
    __outb__(0x3d4, 0x0e);
    uint16_t pos = __inb__(0x3d5);
    pos <<= 8;

    __outb__(0x3d4, 0x0f);
    pos |= __inb__(0x3d5);

    return pos;
}

void svga_SetCursorPos(uint16_t x, uint16_t y) {
    uint16_t pos = y * resolution_x + x;
    __svga_SetCursorPos__(pos);
    return;
}

void __svga_SetCursorPos__(uint16_t cursor_pos) {
    __outb__(0x3d4, 0x0e);
    __outb__(0x3d5, (uint8_t)(cursor_pos >> 8));
    __outb__(0x3d4, 0x0f);
    __outb__(0x3d5, (uint8_t)cursor_pos);

    return;
}

void svga_MoveCursor(uint16_t offset, bool forward) {
    uint16_t cursor_pos = svga_GetCursorPos();
    if (forward)
        cursor_pos += offset;
    else
        cursor_pos -= offset;

    __svga_SetCursorPos__(cursor_pos);
    return;
}

void svga_Clear() {
    char *p = (char *)0xb8000;
    for (int i = 0; i < 2000; i++) {
        *p = 0x00;
        *(p + 1) = 0x0f;
        p += 2;
    }

    svga_SetCursorPos(0, 0);
    return;
}

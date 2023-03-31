//
// File: vga.h
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_VGA_H
#define _BIG_VGA_H

#include <bigos/types.h>

#define VGA_TEXT_FB_BASE 0xb8000                            // vga text mode frame buffer base
#define VGA_TEXT_FB_SIZE (0xb8fff - VGA_TEXT_FB_BASE + 1)   // vga text mode frame buffer size

// vga data register ports
#define VGA_ATT_W  0x3c0   // attribute controller data write register
#define VGA_MIS_W  0x3c2   // miscellaneous output write register
#define VGA_MIS_R  0x3cc   // miscellaneous output read register
#define VGA_CRT_DC 0x3d5   // CRT controller data register - color emulation

// vga index register ports
#define VGA_ATT_IW 0x3c0   // attribute controller index write register
#define VGA_SEQ_I  0x3c4   // sequencer index
#define VGA_CRT_IC 0x3d4   // CRT controller index - color emulation

namespace vga {
    uint8_t *get_bf_pointer();
    uint16_t get_cursor_position();
    void move_cursor(int16_t offset);
    void set_cursor_position(uint16_t pos);
    void set_cursor_position(uint16_t x, uint16_t y);

    void clear_screen();
    void scroll_screen();

    void write(char c, uint8_t color = 0x0f);
    void write(char c, uint16_t position, uint8_t color = 0x0f);
    void write(char c, uint16_t x, uint16_t y, uint8_t color = 0x0f);

    void write(const char *s, uint8_t color = 0x0f);
    void write(const char *s, uint16_t position, uint8_t color = 0x0f);
    void write(const char *s, uint16_t x, uint16_t y, uint8_t color = 0x0f);
}   // namespace vga

#endif   // _BIG_VGA_H

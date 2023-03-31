//
// File: vga.cc
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <video/vga.h>
#include <bigos/io.h>

ptr8_t vga::get_bf_pointer() {
    uintptr_t addr = 0xb8000;
    addr = addr + (get_cursor_position() << 1);
    return (ptr8_t)addr;
}

uint16_t vga::get_cursor_position() {
    uint16_t pos;
    bigos::outb(VGA_CRT_IC, 0x0e);
    pos = bigos::inb(VGA_CRT_DC);

    pos <<= 8;
    bigos::outb(VGA_CRT_IC, 0x0f);
    pos |= bigos::inb(VGA_CRT_DC);

    return pos;
}

void vga::move_cursor(int16_t offset) {
    uint16_t pos = get_cursor_position() + offset;
    set_cursor_position(pos);
}

void vga::set_cursor_position(uint16_t position) {
    bigos::outb(VGA_CRT_IC, 0x0e);
    bigos::outb(VGA_CRT_DC, (uint8_t)(position >> 8));
    bigos::outb(VGA_CRT_IC, 0x0f);
    bigos::outb(VGA_CRT_DC, (uint8_t)position);
}

void vga::set_cursor_position(uint16_t x, uint16_t y) {
    uint16_t pos = y * 80 + x;
    set_cursor_position(pos);
}

void vga::clear_screen() {
    ptr8_t bf_ptr = (ptr8_t)VGA_TEXT_FB_BASE;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        bf_ptr[i] = 0x20;
        bf_ptr[i + 1] = 0x0f;
    }

    set_cursor_position(0);
}

void vga::scroll_screen() {
    // TODO
}

void vga::write(char c, uint8_t color) {
    uint16_t pos = get_cursor_position();
    write(c, pos, color);
}

void vga::write(char c, uint16_t position, uint8_t color) {
    if (c == '\n') {
        position = (position / 80 + 1) * 80;
        set_cursor_position(position);
    } else if (c == '\t') {
        move_cursor(4);
    } else {
        ptr8_t bf_ptr = (ptr8_t)((position << 1) + VGA_TEXT_FB_BASE);
        bf_ptr[0] = c;
        bf_ptr[1] = color;
        move_cursor(1);
    }
}

void vga::write(char c, uint16_t x, uint16_t y, uint8_t color) {
    uint16_t pos = y * 80 + x;
    write(c, pos, color);
}

void vga::write(const char *s, uint8_t color) {
    uint16_t pos = get_cursor_position();
    write(s, pos, color);
}

void vga::write(const char *s, uint16_t position, uint8_t color) {
    ptr8_t bf_ptr = (ptr8_t)((position << 1) + VGA_TEXT_FB_BASE);
    while (*s != 0) {
        if (*s == '\n') {
            position = get_cursor_position();
            position = (position / 80 + 1) * 80;
            set_cursor_position(position);
            bf_ptr = get_bf_pointer();
        } else if (*s == '\t') {
            move_cursor(4);
            bf_ptr += 8;
        } else {
            bf_ptr[0] = *s;
            bf_ptr[1] = color;

            bf_ptr += 2;
            move_cursor(1);
        }
        s++;
    }
}

void vga::write(const char *s, uint16_t x, uint16_t y, uint8_t color) {
    uint16_t pos = y * 80 + x;
    write(s, pos, color);
}

//
// File: vga.cc
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <video/vga.h>
#include <bigos/io.h>

NAMESPACE_DRIVER_BEG
namespace video {
    namespace vga {
        static DisplayMode mode_3{80, 25, true};

        // TODO support more display mode
        // only support mode_3 yet
        static DisplayMode *mode = &mode_3;
        static uint64_t frame_buffer_base = 0xb8000;

        static inline uint16_t convert_pos(uint8_t __x, uint8_t __y) {
            return mode->width * __y + __x;
        }

        // return the offset of cursor
        static uint16_t get_cursor() {
            uint16_t ret;

            bigos::outb(VGA_CRT_IC, 0x0e);
            ret = bigos::inb(VGA_CRT_DC);

            ret <<= 8;
            bigos::outb(VGA_CRT_IC, 0x0f);
            ret |= bigos::inb(VGA_CRT_DC);

            return ret;
        }

        static void set_cursor(uint16_t __pos) {
            bigos::outb(VGA_CRT_IC, 0x0e);
            bigos::outb(VGA_CRT_DC, (uint8_t)(__pos >> 8));
            bigos::outb(VGA_CRT_IC, 0x0f);
            bigos::outb(VGA_CRT_DC, (uint8_t)__pos);
        }

        static void write(char __ch, uint16_t __pos, uint8_t __color = VT_COLOR_NORMAL) {
            if (__ch == '\n') {
                __pos = (__pos / mode->width + 1) * mode->width;
                set_cursor(__pos);
                return;
            }

            if (__ch == '\t') {
                move_cursor(4);
                return;
            }

            ptr8_t fb_ptr = (ptr8_t)((__pos << 1) + frame_buffer_base);
            fb_ptr[0] = __ch;
            fb_ptr[1] = __color;
            move_cursor();
        }

        static void write(const char *__s, uint16_t __pos, uint8_t __color = VT_COLOR_NORMAL) {
            ptr8_t fb_ptr = (ptr8_t)((__pos << 1) + frame_buffer_base);
            set_cursor(__pos);

            while (*__s != 0) {
                char c = *__s;
                ++__s;

                if (c == '\n') {
                    __pos = get_cursor();
                    __pos = (__pos / mode->width + 1) * mode->width;

                    set_cursor(__pos);
                    fb_ptr = (ptr8_t)((__pos << 1) + frame_buffer_base);
                    continue;
                }

                if (c == '\t') {
                    move_cursor(4);
                    fb_ptr += 8;
                    continue;
                }

                fb_ptr[0] = c;
                fb_ptr[1] = __color;

                fb_ptr += 2;
                move_cursor();
            }
        }

        void set_cursor(uint8_t __x, uint8_t __y) {
            set_cursor(convert_pos(__x, __y));
        }

        void move_cursor(int16_t __offset) {
            uint16_t pos = get_cursor() + __offset;
            set_cursor(pos);
        }

        void write(char __ch, uint8_t __color) {
            write(__ch, get_cursor(), __color);
        }

        void write(char __ch, uint8_t __x, uint8_t __y, uint8_t __color) {
            write(__ch, convert_pos(__x, __y), __color);
        }

        void write(const char *__s, uint8_t __color) {
            write(__s, get_cursor(), __color);
        }

        void write(const char *__s, uint8_t __x, uint8_t __y, uint8_t __color) {
            write(__s, convert_pos(__x, __y), __color);
        }

        void clear_screen() {
            ptr8_t fb_ptr = (ptr8_t)frame_buffer_base;
            uint32_t nr_char = mode->width * mode->height;

            while (nr_char--) {
                fb_ptr[0] = 0x20;
                fb_ptr[1] = VT_COLOR_NORMAL;

                fb_ptr += 2;
            }

            set_cursor(0);
        }

        // TODO void scroll_screen(int16_t __offset) {}
    }   // namespace vga
}   // namespace video
NAMESPACE_DRIVER_END

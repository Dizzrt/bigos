//
// File: io.h
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <stdarg.h>
#include <string.h>
#include <bigos/io.h>
#include <video/vga.h>
#include <bigos/utils.h>

uint8_t bigos::inb(uint16_t port) {
    uint8_t ret;

    asm volatile("inb %w1,%b0" : "=a"(ret) : "d"(port));
    return ret;
}

uint16_t bigos::inw(uint16_t port) {
    uint16_t ret;

    asm volatile("inw %w1,%w0" : "=a"(ret) : "d"(port));
    return ret;
}

void bigos::outb(uint16_t port, uint8_t value) {
    asm volatile("outb %b1,%w0" : : "d"(port), "aN"(value));
    return;
}

void bigos::kput(char c) {
    vga::write(c);
}

void bigos::kputs(const char* s) {
    vga::write(s);
}

static uint8_t buffer_append(char* buffer, char* str, uint32_t offset) {
    uint32_t soffset = 0;
    uint32_t slen = strlen(str);
    uint32_t ret = (offset + slen) % 256;

    while (slen > 0xff - offset) {
        uint32_t len = 0xff - offset;
        memcpy(buffer, str + soffset, len);
        bigos::kputs(buffer);
        memset(buffer, 0, sizeof(char) * 256);

        slen -= len;
        soffset += len;
    }

    if (slen > 0)
        memcpy(buffer + offset, str + soffset, slen);

    return ret;
}

void bigos::kprintf(const char* fmt, ...) {
    va_list vlist;
    va_start(vlist, fmt);

    char buffer[256];
    uint32_t offset_buf = 0;
    memset(buffer, 0, sizeof(char) * 256);

    while (*fmt != 0) {
        if (fmt[0] == '%') {
            if (fmt[1] == 'c') {
                char c[2] = {0};
                c[0] = va_arg(vlist, int);
                offset_buf = buffer_append(buffer, c, offset_buf);
            } else if (fmt[1] == 's') {
                char* s = (char*)va_arg(vlist, long long);
                offset_buf = buffer_append(buffer, s, offset_buf);
            } else {
                // may be a number
                bool is_long = false;
                char nbuffer[32] = {0};
                char* nbuffer_ptr = (char*)&nbuffer;

                if (fmt[1] == 'l' && fmt[2] == 'l') {
                    is_long = true;
                    fmt += 2;
                }

                int radix = 10;
                switch (fmt[1]) {
                    case 'd':
                        break;
                    case 'x':
                        radix = 16;
                        nbuffer_ptr[0] = '0';
                        nbuffer_ptr[1] = 'x';
                        nbuffer_ptr += 2;
                        break;
                    default:
                        fmt++;
                        goto NORMAL_CHAR;
                        break;
                }

                if (is_long)
                    itoa(va_arg(vlist, int64_t), nbuffer_ptr, radix);
                else
                    itoa(va_arg(vlist, int32_t), nbuffer_ptr, radix);
                offset_buf = buffer_append(buffer, nbuffer, offset_buf);
            }

            fmt += 2;
        } else {
        NORMAL_CHAR:
            if (offset_buf < 255)
                buffer[offset_buf++] = *fmt++;
            else {
                kputs(buffer);
                memset(buffer, 0, sizeof(char) * 256);
                buffer[0] = *fmt++;
                offset_buf = 0;
            }
        }
    }

    if (offset_buf > 0)
        kputs(buffer);
}

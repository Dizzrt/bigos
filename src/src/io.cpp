/*
 * @Author: Dizzrt
 * @Date: 2021-11-04 12:44:43
 * @LastEditTime: 2021-11-29 13:54:59
 * @LastEditors: Dizzrt
 * @FilePath: \bigos\src\src\io.cpp
 * @Description:
 */
#include "io.h"
#include "dev/svga.h"
#include "global.h"

uint8_t __inb__(uint16_t port) {
    uint8_t ret;

    asm volatile("inb %w1,%b0" : "=a"(ret) : "d"(port));
    return ret;
}

void __outb__(uint16_t port, uint8_t value) {
    asm volatile("outb %b1,%w0" : : "d"(port), "aN"(value));
    return;
}

void __put_char__(uint8_t val) {
    // this long long means to avoid warning which is "cast to pointer from integer of different size"
    uint8_t *p = (uint8_t *)(long long)((svga_GetCursorPos() << 1) + 0x40000b8000);

    if (val == '\n') {
        uint16_t pos = svga_GetCursorPos();
        pos /= resolution_x;
        pos = (pos + 1) * resolution_x;
        __svga_SetCursorPos__(pos);
        return;
    } else if (val == '\t') {
        svga_MoveCursor(4, true);
        return;
    }

    *p = val;
    *(p + 1) = 0x0f;
    svga_MoveCursor(1, true);
    return;
}

void __put_int__(uint64_t val, INT_MODE mode) {
    uint8_t i = 0;
    uint8_t buf[64] = {0};

    uint8_t mod;
    switch (mode) {
        case INT_MODE::HEX:
            mod = 16;
            __put_char__('0');
            __put_char__('x');
            break;
        case INT_MODE::DEC: mod = 10; break;
        case INT_MODE::OCT: mod = 8; break;
        case INT_MODE::BIN: mod = 2; break;
        default: mod = 16; break;
    }

    while (val > 0) {
        buf[i++] = val % mod;
        val /= mod;
    }

    if (i == 0) {
        __put_char__('0');
        return;
    }

    do {
        --i;
        if (buf[i] > 9)
            __put_char__(buf[i] - 10 + 'A');
        else
            __put_char__(buf[i] + '0');
    } while (i > 0);

    return;
}

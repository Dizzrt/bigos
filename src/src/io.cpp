/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-17 21:09:08
 */

#include "io.h"
#include "dev/svga.h"
#include "string.h"

uint8_t __inb__(uint16_t port) {
    uint8_t ret;

    asm volatile("inb %w1,%b0" : "=a"(ret) : "d"(port));
    return ret;
}

void __outb__(uint16_t port, uint8_t value) {
    asm volatile("outb %b1,%w0" : : "d"(port), "aN"(value));
    return;
}

static inline uint8_t *__getSvagPointer() {
    return (uint8_t *)(long long)((svga_GetCursorPos() << 1) + 0xffff8000000b8000);
}

static inline void __tab() { return svga_MoveCursor(TAB_WIDTH, true); }

static inline uint8_t *__enter() {
    uint16_t pos = svga_GetCursorPos();
    pos /= resolution_x;
    pos = (pos + 1) * resolution_x;
    __svga_SetCursorPos__(pos);
    return __getSvagPointer();
}

void putk_svag(char val) {
    if (val == '\n')
        __enter();
    else if (val == '\t')
        __tab();
    else {
        uint8_t *p = __getSvagPointer();
        *p = val;
        *(p + 1) = 0x0f;
        svga_MoveCursor(1, true);
    }
    return;
}

void putsk_svga(const char *str) {
    uint16_t curPosDelta = 0;
    uint8_t *p = __getSvagPointer();

    while (*str != 0) {
        if (*str == '\n')
            p = __enter(), curPosDelta = 0;
        else if (*str == '\t')
            __tab(), p += TAB_WIDTH * 2;
        else {
            *p = *str;
            *(p + 1) = 0x0f;
            p += 2;
            curPosDelta += 1;
        }

        str++;
    }

    if (curPosDelta > 0)
        svga_MoveCursor(curPosDelta, true);
    return;
}

static uint8_t updateBuffer(char *buffer, char *val, uint8_t offset) {
    uint32_t val_offset = 0;
    uint32_t val_l = strlen(val);

    uint8_t ret = (offset + val_l) % 256;

    while (val_l > 0xff - offset) {
        uint8_t len = 0xff - offset;
        memcpy(buffer, val + val_offset, len);
        putsk_svga(buffer);
        memset(buffer, 0, sizeof(char) * 256);

        val_l -= len;
        val_offset += len;
    }

    if (val_l > 0)
        memcpy(buffer + offset, val + val_offset, val_l);

    return ret;
}

void printk_svga(const char *fmt, ...) {
    va_list valist;
    va_start(valist, fmt);

    uint8_t ptr = 0;
    char buffer[256] = {0};

    bool ll_flag = false;
    while (*fmt != 0) {
        if (*fmt == '%') {
            if (*(fmt + 1) == 'c') {
                char p[2] = {0};
                p[0] = va_arg(valist, int);
                ptr = updateBuffer(buffer, p, ptr);
            } else if (*(fmt + 1) == 's') {
                char *p = (char *)va_arg(valist, long long);
                ptr = updateBuffer(buffer, p, ptr);
            } else { // maby a num
                if (*(fmt + 1) == 'l' && *(fmt + 2) == 'l') {
                    ll_flag = true;
                    fmt += 2;
                }

                if (*(fmt + 1) == 'd') {
                    char tmp[20] = {0};
                    if (ll_flag)
                        itoa(va_arg(valist, int64_t), tmp, 10), ll_flag = false;
                    else
                        itoa(va_arg(valist, int32_t), tmp, 10);
                    ptr = updateBuffer(buffer, tmp, ptr);
                } else if (*(fmt + 1) == 'x') {
                    char tmp[20] = {0};
                    if (ll_flag)
                        itoa(va_arg(valist, int64_t), tmp, 16), ll_flag = false;
                    else
                        itoa(va_arg(valist, int32_t), tmp, 16);
                    ptr = updateBuffer(buffer, tmp, ptr);
                } else {
                    fmt++;
                    goto normalC;
                }
            }

            fmt += 2;
        } else {
        normalC:
            if (ptr < 255)
                buffer[ptr++] = *fmt++;
            else {
                putsk_svga(buffer);
                memset(buffer, 0, sizeof(char) * 256);
                buffer[0] = *fmt++;
                ptr = 0;
            }
        }
    }

    if (ptr > 0)
        putsk_svga(buffer);

    va_end(valist);
    return;
}

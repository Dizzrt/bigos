//
// File: io.h
// Created by Dizzrt on 2023/03/28.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_IO_H
#define _BIG_IO_H

#include <bigos/types.h>

namespace bigos {
    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);
    void outb(uint16_t port, uint8_t value);

    void kput(char c);
    void kputs(const char *s);
    void kprintf(const char *fmt, ...);

}   // namespace bigos

#endif
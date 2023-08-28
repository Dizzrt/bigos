//
// File: kernel.cc
// Created by Dizzrt on 2023/03/23.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

// cpp version check
#if __cplusplus < 201703L
#warning C++ 17 is recommended
#endif

#ifndef __GNUC__
#warning It is recommended to build with GCC
#endif

#include <video/vga.h>

#include <memory.h>
#include <irq/interrupt.h>

#include <bigos/io.h>
#include <ktl/buffer.h>

extern "C" void kernel();

void kernel() {
    driver::video::vga::clear_screen();

    bigos::init_mem();
    bigos::irq::init_irq();
    // bigos::terminal::init_tty();
    bigos::irq::enable_irq();

    while (true) {
        asm volatile("hlt");
    }
}

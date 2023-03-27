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

extern "C" void kernel();

void kernel() {
    while (true) {
        asm volatile("hlt");
    }
}

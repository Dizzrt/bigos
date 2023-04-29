//
// File: interrupt.cc
// Created by Dizzrt on 2023/04/26.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <irq/interrupt.h>
#include <irqchip/i8259.h>

NAMESPACE_BIGOS_BEG
namespace irq {
    void init_irq() {
        isr::__detail::init_idt();
        __detail::init_i8259();

        enable_irq();
    }
}   // namespace irq
NAMESPACE_BIGOS_END

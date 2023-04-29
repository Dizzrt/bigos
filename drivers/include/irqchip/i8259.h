//
// File: i8259.h
// Created by Dizzrt on 2023/04/26.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_I8259_H
#define _BIG_I8259_H

#include <bigos/types.h>

#define I8259_MASTER_ICW1 0x20
#define I8259_MASTER_ICW2 0x21
#define I8259_MASTER_ICW3 0x21
#define I8259_MASTER_ICW4 0x21
#define I8259_MASTER_OCW1 0x21
#define I8259_MASTER_OCW2 0x20
#define I8259_MASTER_OCW3 0x20

#define I8259_SLAVE_ICW1 0xa0
#define I8259_SLAVE_ICW2 0xa1
#define I8259_SLAVE_ICW3 0xa1
#define I8259_SLAVE_ICW4 0xa1
#define I8259_SLAVE_OCW1 0xa1
#define I8259_SLAVE_OCW2 0xa0
#define I8259_SLAVE_OCW3 0xa0

#define I8259_IRQ_TIMER         0
#define I8259_IRQ_KEYBOARD      1
#define I8259_IRQ_SLAVE         2
#define I8259_IRQ_COMB          3
#define I8259_IRQ_COMA          4
#define I8259_IRQ_LPTB          5
#define I8259_IRQ_FLOPPY        6
#define I8259_IRQ_LPTA          7
#define I8259_IRQ_RTC           8
#define I8259_IRQ_PS2_MOUSE     12
#define I8259_IRQ_FPU           13
#define I8259_IRQ_PRIMARY_IDE   14
#define I8259_IRQ_SECONDARY_IDE 15

#define I8259_EOI      0x20
#define I8259_IRQ_BASE 0x20

#define i8259_get_irq_num(irq) (irq + I8259_IRQ_BASE)

NAMESPACE_BIGOS_BEG
namespace irq {
    namespace __detail {
        void init_i8259() noexcept;
    }   // namespace __detail

    void i8259_init_imr(uint16_t __irq_num) noexcept;
    void i8259_set_imr(uint8_t __irq_num) noexcept;
    void i8259_reset_imr(uint8_t __irq_num) noexcept;

    void i8259_EOI(uint16_t __irq_num) noexcept;
}   // namespace irq
NAMESPACE_BIGOS_END
#endif   // _BIG_I8259_H

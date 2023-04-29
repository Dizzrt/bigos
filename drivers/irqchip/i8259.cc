//
// File: i8259.cc
// Created by Dizzrt on 2023/04/26.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <irqchip/i8259.h>

#include <bigos/io.h>
#include <bigos/types.h>

static uint16_t init_mask = 0xffff;

NAMESPACE_BIGOS_BEG
namespace irq {
    namespace __detail {
        void init_i8259() {
            // master
            outb(I8259_MASTER_ICW1, 0x11);
            outb(I8259_MASTER_ICW2, 0x20);
            outb(I8259_MASTER_ICW3, 0x04);
            outb(I8259_MASTER_ICW4, 0x01);
            outb(I8259_MASTER_OCW1, init_mask);

            // slave
            outb(I8259_SLAVE_ICW1, 0x11);
            outb(I8259_SLAVE_ICW2, 0x28);
            outb(I8259_SLAVE_ICW3, 0x02);
            outb(I8259_SLAVE_ICW4, 0x01);
            outb(I8259_SLAVE_OCW1, (init_mask >> 8));
        }
    }   // namespace __detail

    void i8259_init_imr(uint16_t __irq_num) {
        if (__irq_num > 0x0f)
            return;

        uint16_t mask = 1 << __irq_num;
        init_mask &= ~mask;
    }

    void i8259_set_imr(uint8_t __irq_num) {
        uint8_t value;
        uint16_t port;

        if (__irq_num < 8) {
            port = I8259_MASTER_OCW1;
        } else {
            port = I8259_SLAVE_OCW1;
            __irq_num -= 8;
        }

        value = inb(port) | (1 << __irq_num);
        outb(port, value);
    }

    void i8259_reset_imr(uint8_t __irq_num) {
        uint8_t value;
        uint16_t port;

        if (__irq_num < 8) {
            port = I8259_MASTER_OCW1;
        } else {
            port = I8259_SLAVE_OCW1;
            __irq_num -= 8;
        }

        value = inb(port) & ~(1 << __irq_num);
        outb(port, value);
    }

    void i8259_EOI(uint16_t __irq_num) {
        if (__irq_num >= 8)
            outb(I8259_SLAVE_OCW2, I8259_EOI);

        outb(I8259_MASTER_OCW2, I8259_EOI);
    }
}   // namespace irq
NAMESPACE_BIGOS_END

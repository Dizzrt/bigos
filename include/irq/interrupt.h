//
// File: interrupt.h
// Created by Dizzrt on 2023/04/26.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_INTERRUPT_H
#define _BIG_INTERRUPT_H

#include <bigos/types.h>

#define IDT_SIZE    0x1000
#define IDT_BASE    0x1000ul
#define MAX_IRQ_NUM (IDT_SIZE / sizeof(bigos::irq::IntrDescriptor) - 1)

NAMESPACE_BIGOS_BEG
namespace irq {
    namespace __detail {
        void init_idt() noexcept;

        struct IntrDescriptorAttributes {
            uint16_t ist : 3;
            uint16_t reserved : 5;
            uint16_t type : 5;
            uint16_t dpl : 2;
            uint16_t p : 1;
        } __attribute__((packed));
    }   // namespace __detail

    struct IntrDescriptor {
        uint16_t offset_low;
        uint16_t selector;
        union {
            uint16_t attributes;
            __detail::IntrDescriptorAttributes attributes_detail;
        };
        uint16_t offset_mid;
        uint32_t offset_high;
        uint32_t reserved;

        IntrDescriptor() = default;
        IntrDescriptor(void* __addr) {
            uint64_t addr = (uint64_t)__addr;
            offset_low = addr;
            offset_mid = addr >> 16;
            offset_high = addr >> 32;
        }
    };

    typedef void (*irq_handler)(uint64_t __irq_num, uint64_t __ecode);

    inline void enable_irq() noexcept {
        asm volatile("sti");
    }

    inline void disable_irq() noexcept {
        asm volatile("cli");
    }

    void init_irq() noexcept;
}   // namespace irq

NAMESPACE_BIGOS_END
#endif   // _BIG_INTERRUPT_H

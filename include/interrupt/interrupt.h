//
// File: interrupt.h
// Created by Dizzrt on 2023/08/31.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#ifndef _BIG_INTERRUPT_H
#define _BIG_INTERRUPT_H

#include <bigos/types.h>

// interrupt descriptor table
#define IDT_SIZE 0x1000
#define IDT_BASE 0x1000ul

NAMESPACE_BIGOS_BEG
namespace intr {
    namespace __detail {
        void initIDT() noexcept;

        struct GateAttributes {
            uint16_t ist : 3;
            uint16_t reserved_0 : 5;
            uint16_t type : 4;
            uint16_t reserved_1 : 1;
            uint16_t dpl : 2;
            uint16_t p : 1;
        } __attribute__((packed));
    }   // namespace __detail

    struct Gate {
        uint16_t offset_low;
        uint16_t selector;
        union {
            uint16_t attributes_brief;
            __detail::GateAttributes attributes;
        };
        uint16_t offset_mid;
        uint32_t offset_high;
        uint32_t reserved;

        void setISR(void *isr) {
            uint64_t isr_address = (uint64_t)isr;
            offset_low = isr_address;
            offset_mid = isr_address >> 16;
            offset_high = isr_address >> 32;
        }
    };

    typedef void (*IRQHandler)(uint64_t irq_number, uint64_t error_code);

    inline void enableIRQ() noexcept {
        asm volatile("sti");
    }

    inline void disableIRQ() noexcept {
        asm volatile("cli");
    }

    void initINTR() noexcept;
}   // namespace intr
NAMESPACE_BIGOS_END
#endif   // _BIG_INTERRUPT_H
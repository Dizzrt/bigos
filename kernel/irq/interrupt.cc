//
// File: interrupt.cc
// Created by Dizzrt on 2023/04/26.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <irq/isr.h>
#include <irq/interrupt.h>
#include <irqchip/i8259.h>

#include <bigos/io.h>

NAMESPACE_BIGOS_BEG
namespace irq {
    namespace __detail {
        extern "C" void* isr_entry_list[MAX_IRQ_NUM];

        static void default_isr(uint64_t __irq_num, uint64_t __ecode) {
            kprintf("default ISR called with irq number: %lld and error code: %lld\n", __irq_num, __ecode);
        }

        void init_idt() {
            irq::IntrDescriptor* idt = (irq::IntrDescriptor*)IDT_BASE;

            for (uint32_t i = 0; i <= MAX_IRQ_NUM; i++) {
                irq::IntrDescriptor id(__detail::isr_entry_list[i]);
                // code 64 selector`
                id.selector = 0x08;
                id.attributes = 0x8e00;
                idt[i] = id;

                isr::isr_list[i] = &__detail::default_isr;
            }
        }
    }   // namespace __detail

    void init_irq() {
        __detail::init_idt();
        driver::irqchip::i8259::init();
        isr::init_isr();
    }
}   // namespace irq
NAMESPACE_BIGOS_END

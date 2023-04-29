//
// File: isr.cc
// Created by Dizzrt on 2023/04/29.
//
// Copyright (C) 2023 The BigOS Authors.
// Licensed under the GNU General Public License v3.0 only.
//

#include <irq/interrupt.h>

#include <string.h>
#include <bigos/io.h>

NAMESPACE_BIGOS_BEG
namespace isr {
    namespace __detail {
        isr_handler isr_list[MAX_IRQ_NUM];
        extern "C" void* isr_entry_list[MAX_IRQ_NUM];

        static void default_isr(uint64_t __irq_num, uint64_t __ecode) {
            kprintf("default ISR called with irq number: %lld\n", __irq_num);
        }

        void init_idt() {
            // clear idt area
            memset((void*)IDT_BASE, 0, IDT_SIZE);

            irq::INTDescriptor* idt = (irq::INTDescriptor*)IDT_BASE;

            for (uint32_t i = 0; i <= MAX_IRQ_NUM; i++) {
                irq::INTDescriptor id(__detail::isr_entry_list[i]);
                // code 64 selector
                id.selector = 0x08;
                id.attributes = 0x8e00;
                idt[i] = id;

                if (__detail::isr_list[i] == nullptr)
                    __detail::isr_list[i] = &__detail::default_isr;
            }
        }
    }   // namespace __detail

    void register_isr(uint64_t __irq_num, isr_handler __isr) {
        if (__irq_num > MAX_IRQ_NUM)
            return;

        __detail::isr_list[__irq_num] = __isr;
    }
}   // namespace isr
NAMESPACE_BIGOS_END

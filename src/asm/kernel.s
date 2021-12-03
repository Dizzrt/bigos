
.file "kernel.s"

.text
.code64
.extern Kernel
.global __kernel
__kernel:
    movabsq $idt_attribute,%rax
    lidt (%rax)

    movq $0x4000004fff,%rsp
    jmp Kernel


.include "intr_entries.s"

.data
idt_attribute:
    .word 0x320
    .quad 0x4000000000

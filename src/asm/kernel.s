
.file "kernel.s"

.text
.code64
.extern Kernel
.global __kernel
__kernel:
    movabsq $idt_attribute,%rax
    lidt (%rax)
    jmp Kernel


.include "intr_entries.s"

.data
idt_attribute:
    .word 0x3ff
    .quad 0x4000000000

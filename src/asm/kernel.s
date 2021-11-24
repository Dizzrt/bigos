
.file "kernel.s"
.extern Kernel

.code64
.section .text 
.global __kernel
__kernel:
    movabsq $idt_attribute,%rax
    lidt (%rax)
    jmp Kernel

idt_attribute:
    .word 0x3ff
    .quad 0x4000000000

.global __iret__
__iret__:
mov $0x20,%dx
mov $0x20,%al 
out %al,%dx
iretq

.extern do_intr

.data
.global intr_entry_table
intr_entry_table:
.macro intr_entry ivec,ecode=0
.text
intr_entry_\ivec:
    .if \ecode==0
        pushq $0
    .endif
    pushq $\ivec
    jmp intr_entry
.data
.quad intr_entry_\ivec
.endm

.text
intr_entry:
    push %rbp
    mov %rsp,%rbp
    push %rdi
    push %rsi
    mov 8(%rbp),%rdi
    mov 16(%rbp),%rsi
    call do_intr
    pop %rsi
    pop %rdi
    pop %rbp
    addq $16,%rsp

    mov $0x20,%dx
    mov $0x20,%al
    outb %al,%dx
    mov $0xa0,%al
    outb %al,%dx
    iretq

intr_entry 0x00
intr_entry 0x01
intr_entry 0x02
intr_entry 0x03
intr_entry 0x04
intr_entry 0x05
intr_entry 0x06
intr_entry 0x07
intr_entry 0x08,1
intr_entry 0x09
intr_entry 0x0a,1
intr_entry 0x0b,1
intr_entry 0x0c,1
intr_entry 0x0d,1
intr_entry 0x0e,1
intr_entry 0x0f
intr_entry 0x10
intr_entry 0x11,1
intr_entry 0x12
intr_entry 0x13

#----reserved----#
intr_entry 0x14
intr_entry 0x15
intr_entry 0x16
intr_entry 0x17
intr_entry 0x18
intr_entry 0x19
intr_entry 0x1a
intr_entry 0x1b
intr_entry 0x1c
intr_entry 0x1d
intr_entry 0x1e
intr_entry 0x1f
#----reserved----#

#----IRQ begin----#
intr_entry 0x20
intr_entry 0x21
intr_entry 0x22
intr_entry 0x23
intr_entry 0x24
intr_entry 0x25
intr_entry 0x26
intr_entry 0x27
intr_entry 0x28
intr_entry 0x29
intr_entry 0x2a
intr_entry 0x2b
intr_entry 0x2c
intr_entry 0x2d
intr_entry 0x2e
intr_entry 0x2f
#----IRQ end----#

#soft irq entry
intr_entry 0x30

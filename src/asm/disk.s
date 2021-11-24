.code64
read_one_sector:
    mov $1,%al
    mov $0x1f2,%dx
    outb %al,%dx
    
    movq 8(%rsp),%rax
    
    mov $0x1f3,%dx
    outb %al,%dx
    shr $8,%rax

    mov $0x1f4,%dx
    outb %al,%dx
    shr $8,%rax

    mov $0x1f5,%dx
    outb %al,%dx

    mov $0x1f6,%dx
    mov $0xe0,%al
    outb %al,%dx

    mov $0x20,%al
    mov $0x1f7,%dx
    outb %al,%dx

reading:
    nop
    inb %dx,%al
    andb $0x88,%al
    cmpb $0x08,%al
    jne reading

write:
    movq 16(%rsp),%rbx
    mov $0x1f0,%dx
    mov $256,%rcx
writing:
    inw %dx,%ax
    movw %ax,(%rbx)
    addq $2,%rbx
    loop writing
    
    ret

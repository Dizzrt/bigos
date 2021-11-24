.file "boot"
.text
.code16
.global boot
boot:
    mov $0,%ax
    mov %ax,%ds
    mov %ax,%es
detect_memory:
    mov $0,%ebx
    mov $0x7e00,%di
detect_memory_Loop:
    mov $20,%ecx
    mov $0xe820,%eax
    mov $0x534d4150,%edx
    int $0x15
    jc error
    movl 16(%di),%eax
    cmp $1,%eax
    jne L0
    add $20,%di
L0:
    cmp $0,%ebx
    jne detect_memory_Loop

jmp build_gdt
gdt_attribute:
    .word 55 #7*8-1
    .quad 0x7c00

build_gdt:
    mov $0x7c00,%bx
.equ SELECTOR_NULL,0x0<<3
    movl $0x000000,0x0(%bx)
    movl $0x000000,0x4(%bx)
.equ SELECTOR_CODE,0x1<<3 
    movl $0x0000ffff,0x8(%bx)
    movl $0x00cf9800,0xc(%bx)
.equ SELECTOR_DATA,0x2<<3
    movl $0x0000ffff,0x10(%bx)
    movl $0x00cf9200,0x14(%bx)
.equ SELECTOR_CODE_x64,0x3<<3
    movl $0x0000ffff,0x18(%bx)
    movl $0x00af9800,0x1c(%bx)
.equ SELECTOR_DATA_x64,0x4<<3
    movl $0x0000ffff,0x20(%bx)
    movl $0x00af9200,0x24(%bx)
.equ SELECTOR_STACK_x64,0x5<<3
    movl $0x00000500,0x28(%bx)
    movl $0x00209600,0x2c(%bx)
.equ SELECTOR_VIDEO_x64,0x6<<3
    movl $0x00000500,0x30(%bx)
    movl $0x00209600,0x34(%bx)

enter_protected:
    lgdt (gdt_attribute)

    inb $0x92,%al
    orb $0x2,%al
    outb %al,$0x92

    movl %cr0,%eax
    orl $0x1,%eax
    movl %eax,%cr0

    cli 
    ljmp $SELECTOR_CODE,$(protected_mod)

.code32
protected_mod:
    mov $SELECTOR_DATA,%ax
    mov %ax,%ds

    movl $0x0000903f,(0x8000) #PML4
    movl $0x00000000,(0x8004)

    mov $0x200,%ecx
    movl $0x9000,%ebx
L1:
    movl $0x00000000,(%ebx)
    movl $0x00000000,4(%ebx)
    add $8,%ebx
    loop L1 #pdpt清0

# #0 PD of pdpt
    movl $0x0000a03f,(0x9000)  
# #256 PD of pdpt
    movl $0x0000a03f,(0x9800) 

# #0-31 pt of #256(0)pd
    movl $32,%ecx
    movl $0xa000,%ebx
    movl $0x0000b03f,%edx
L2:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    add $8,%ebx
    add $0x1000,%edx
    loop L2

#the 32 PTs of #256 PD    
    movl $0x4000,%ecx
    movl $0xb000,%ebx
    movl $0x000001ff,%edx
L3:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    add $8,%ebx
    add $0x1000,%edx
    loop L3

enter_long:
    mov $0x8000,%eax
    mov %eax,%cr3

    mov %cr4,%eax
    or $0x20,%eax
    mov %eax,%cr4

    mov $0x0c0000080,%ecx
    rdmsr
    bts $8,%eax
    wrmsr

    mov %cr0,%eax
    bts $31,%eax
    mov %eax,%cr0

    ljmp $SELECTOR_CODE_x64,$(long_mod)

.code64
long_mod:
    mov $0x4000000000,%rbx
    addq %rbx,(gdt_attribute+2)
    lgdt (gdt_attribute)
    
    movw $SELECTOR_STACK_x64,%ax #init stack
    movw %ax,%ss
    movq $0x4000007bff,%rsp

    movw $SELECTOR_DATA_x64,%ax
    movw %ax,%ds

load_kernel:
    movq $0,%rax
    mov $0x4000100000,%r8
load_kernel_Loop:
    movl (0x500),%eax
    push %r8
    push %rax
    call read_one_sector
    addq $16,%rsp

    movl 508(%r8),%eax
    cmpl $0x456e644c,%eax
    jne continue_load_kernel
    movl 504(%r8),%eax
    cmpl $0x6f616400,%eax
    jne continue_load_kernel

enter_kernel:
    mov $0x4000100000,%rax
    jmp *%rax

continue_load_kernel:
    add $0x200,%r8
    incb (0x500)
    jmp load_kernel_Loop

io_delay:
    push %rcx
    movq $8,%rcx
io_delay_loop: #TODO xxx
    nop
    loop io_delay_loop
    pop %rcx
    ret

error:
    jmp .

.include "disk.s"

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
    mov $0x7c00,%di
    mov $0x508,%si
detect_memory_Loop:
    mov $20,%ecx
    mov $0xe820,%eax
    mov $0x534d4150,%edx
    int $0x15
    jc error
    movl 16(%di),%eax
    cmp $1,%eax
    jne L0
    movl (%di),%eax
    movl %eax,(%si)
    movl 4(%di),%eax
    movl %eax,4(%si)
    movl 8(%di),%eax
    movl %eax,8(%si)
    movl 12(%di),%eax
    movl %eax,12(%si)
    add $16,%si
    incl ams
L0:
    cmp $0,%ebx
    jne detect_memory_Loop
    movl ams,%eax
    movl %eax,0x504

jmp build_gdt
ams: .long 0 #available memory segment count

gdt_attribute:
    .word 255 
    .quad 0x300

gdt_attribute_x64:
    .word 255
    .quad 0xffff800000000300

idt_attribute:
    .word 0x300
    .quad 0xffff800000000000

build_gdt:
    mov $0x300,%bx
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
    movl $0x00003000,0x28(%bx)
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

    movl $0x0000903f,(0x8000) #PML4[0]
    movl $0x00000000,(0x8004)
    movl $0x0000903f,(0x8800) #PML4[256]
    movl $0x00000000,(0x8804)

    movl $0x0000a03f,(0x9000) #PDPT[0]
    movl $0x00000000,(0x9004)
 
    movl $4,%ecx              #PD[0-3]
    movl $0xa000,%ebx
    movl $0x0000b03f,%edx
L1:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    add $8,%ebx
    add $0x1000,%edx
    loop L1
   
    movl $0x800,%ecx          #the 4 PTs
    movl $0xb000,%ebx
    movl $0x000001ff,%edx
L2:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    add $8,%ebx
    add $0x1000,%edx
    loop L2

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
    lgdt (gdt_attribute_x64)

    movw $SELECTOR_STACK_x64,%ax #init stack
    movw %ax,%ss
    movq $0xffff800000006fff,%rsp

    movw $SELECTOR_DATA_x64,%ax
    movw %ax,%ds
    movw %ax,%es

    movabsq $idt_attribute,%rax
    lidt (%rax)

load_kernel:
    movq $0,%r9
    movq $0,%rax
    mov $0xffff800000100000,%r8
load_kernel_Loop:
    incq %r9
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
    mov $0xffff800000100000,%rax
    jmp *%rax

continue_load_kernel:
    add $0x200,%r8
    incl (0x500)
    jmp load_kernel_Loop

error:
    jmp .

.include "disk.s"

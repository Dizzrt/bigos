.file "boot.s"
.code16
.text

head:
    jmp boot
.include "error.s"

DiskAddressPacket:
    .byte 0x10      #PackSize
    .byte 0         #Reserved
    .word 1         #BlockCount
    .word 0x8000    #BufferOffset
    .word 0         #BufferSegment
    .long 0         #BlockLow
    .long 0         #BlockHigh

KernelSize: .long 0 #number of sectors
KernelLBALow: .long 0 
KernelLBAHigh: .long 0 

gdt_attribute:
    .word 0xff
    .quad gdt_32bit

#temp gdt for 32-bit
gdt_32bit:
    .quad 0x0000000000000000
    .quad 0x00cf98000000ffff
    .quad 0x00cf92000000ffff
    .quad 0x00af98000000ffff

gdt_attribute_x64:
    .word 0xff
    .quad 0xffff800000000300

idt_attribute:
    .word 0x300
    .quad 0xffff800000000000

.global boot
boot:
    #detect memory
    movl $0,%ebx
    movw $0x7010,%di
    movl $0x20,%ecx
    movb $0x34,(error_code)
    xor %si,%si
L0:
    movl $0xe820,%eax
    movl $0x534d4150,%edx
    int $0x15
    jc error
    incw %si
    movl 16(%di),%eax
    cmpl $1,%eax
    jne L1
    addw $0x14,%di
L1:
    cmpl $0,%ebx
    jne L0
    movw %si,%ax
    movb %al,(0x7000)
    #----search kernel----
    #read mbr
    movb $0x42,%ah
    movb $0x80,%dl
    movw $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    #search active partition
    movw $4,%cx
    movw $0x81be,%di
L2: #searching
    movb (%di),%bl
    cmpb $0x80,%bl
    je L3
    addw $0x10,%di
    loop L2
    movb $0x31,(error_code)
    jmp error
L3: #dbr found
    movl 8(%di),%ebx #LBA of partition start
    movl %ebx,(DiskAddressPacket+8)
    movl $0x9000,%ebx #load dbr into 0x9000
    movl %ebx,(DiskAddressPacket+4)

    #read dbr
    movb $0x42,%ah
    movb $0x80,%dl
    movw $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    #end read

    xor %ebx,%ebx
    movw $0x9000,%di
    movw 0x0e(%di),%bx
    movl 0x1c(%di),%eax
    addl %eax,%ebx
    movl 0x24(%di),%eax
    xor %cx,%cx
    movb 0x10(%di),%cl
L4: #FAT size * FAT count
    addl %eax,%ebx
    loop L4
    movl %ebx,(DiskAddressPacket+8) #LBA of data aera
    movl $0x8000,(DiskAddressPacket+4) #load data aera into 0x8000~0x8fff
    movw $0x08,(DiskAddressPacket+2)
    #read data aera
    movb $0x42,%ah
    movb $0x80,%dl
    movw $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    #read end
    movw $0x8000,%di
    movb $0x35,(error_code)
L5: #searching kernel
    cmpl $0x4e52454b,(%di)
    jne L6
    cmpl $0x20204c45,4(%di)
    jne L6
    cmpw $0x2020,8(%di)
    jne L6
    cmpb $0x20,10(%di)
    je L7
L6: #next file
    cmpw $0x9000,%di
    ja error
    addw $0x20,%di
    jmp L5
L7: #kernel found
    movl $0x200,%ecx
    xor %edx,%edx
    movl 0x1c(%di),%eax
    divl %ecx
    cmpl $0,%edx
    je L8
    incl %eax
L8: #no ramainder
    movl %eax,(KernelSize) # serctors

    #calcutate LBA of kernel
    movw 0x14(%di),%ax
    shl $0x10,%eax
    movw 0x1a(%di),%ax
    subl $2,%eax
    xor %ebx,%ebx
    movb (0x900d),%bl
    mull %ebx
    addl (DiskAddressPacket+8),%eax
    movl %edx,(KernelLBAHigh)
    movl %eax,(KernelLBALow)
    #----end search kernel----

    #enter protected mod
    lgdt (gdt_attribute)

    inb $0x92,%al
    orb $0x02,%al
    outb %al,$0x92

    movl %cr0,%eax
    orl $0x01,%eax
    movl %eax,%cr0

    cli
    ljmp $0x08,$(protected)

.code32    
protected:
    movw $0x10,%ax
    movw %ax,%ds
    
    xor %edx,%edx
    movl (KernelSize),%eax
    movl $0x08,%ecx
    divl %ecx
    cmpl $0,%edx
    je L9
    incl %eax #pages of kernel
L9: #no remainder
    xor %edx,%edx
    movl $0x200,%ecx
    divl %ecx
    cmpl $0,%edx
    je L10
    incl %eax
L10: #no ramainder
    incl %eax #add the low 2MB
    cmpl $0x100,%eax
    jbe L11
    #kernel is too big 
    movb $0x65,(error_code)
    jmp .#error

L11:
    movl $0x0000903f,(0x8000) #pml4[0]
    movl $0x00000000,(0x8004)
    movl $0x0000903f,(0x8800) #pml4[256]
    movl $0x00000000,(0x8804)

    movl $0x0000a03f,(0x9000) #pdpt[0]
    movl $0x00000000,(0x9004)
    
    #pd
    movl %eax,%ecx
    movl $0x0000a000,%ebx
    movl $0x0010003f,%edx
L12:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    addl $0x08,%ebx
    addl $0x1000,%edx
    loop L12

    #pt
    movl $0x200,%ecx
    mull %ecx
    movl %eax,%ecx
    movl $0x100000,%ebx
    movl $0x000001ff,%edx
L13:
    movl %edx,(%ebx)
    movl $0x00000000,4(%ebx)
    addl $0x08,%ebx
    addl $0x1000,%edx
    loop L13

    #enter long mod

    #build 64bit gdt
    movw $0x300,%bx
    movl $0x00000000,0x00(%bx) #zero
    movl $0x00000000,0x04(%bx)
    movl $0x0000ffff,0x08(%bx) #code
    movl $0x00af9800,0x0c(%bx)
    movl $0x0000ffff,0x10(%bx) #data
    movl $0x00af9200,0x14(%bx)
    movl $0x00003000,0x18(%bx) #stack
    movl $0x00209600,0x1c(%bx)
    movl $0x00000500,0x20(%bx) #video
    movl $0x00209600,0x24(%bx)

    .equ SELECTOR_CODE_x64,0x1<<3
    .equ SELECTOR_DATA_x64,0x2<<3
    .equ SELECTOR_STACK_x64,0x3<<3
    .equ SELECTOR_VIDEO_x64,0x4<<3

    movl $0x8000,%eax
    movl %eax,%cr3

    movl %cr4,%eax
    bts $0x05,%eax
    movl %eax,%cr4 #cr4.PAE=1

    movl $0xc0000080,%ecx #0xc0000080 ==> IA32_EFFR
    rdmsr
    bts $0x08,%eax #IA32_EFFR.LME=1
    wrmsr

    movl %cr0,%eax
    #bts $0x00,%eax
    bts $0x1f,%eax
    movl %eax,%cr0

    ljmp $0x18,$(long_gate)

.code64
long_gate:
    #init
    pushf           #flags
    pushq $0x08     #cs
    pushq $long     #ip
    lgdt (gdt_attribute_x64)
    iretq
long:
    movw $SELECTOR_STACK_x64,%ax
    movw %ax,%ss
    movq $0xffff800000006fff,%rsp

    movw $SELECTOR_DATA_x64,%ax
    movw %ax,%ds
    movw %ax,%es

    movabsq $idt_attribute,%rax
    lidt (%rax)

    xor %rax,%rax
    movq %rax,%r9
    movq %rax,%rcx
    movl (KernelSize),%eax
    movq %rax,%r8
    movq $0x200000,%r10
loadKernel:
    cmpq $0xffff,%r8
    jbe less
    subq $0xffff,%r8
    movq $0xffff,%r9
    jmp loading
less:
    movq %r8,%r9
    xor %r8,%r8
loading:
    movb $0x40,%al
    movw $0x01f6,%dx
    outb %al,%dx

    movq %r9,%rax
    shr $0x08,%rax
    movw $0x01f2,%dx
    outb %al,%dx

    movb (KernelLBALow+3),%al
    movw $0x1f3,%dx
    outb %al,%dx

    movb (KernelLBAHigh),%al
    movw $0x1f4,%dx
    outb %al,%dx

    movb (KernelLBAHigh+1),%al
    movw $0x1f5,%dx
    outb %al,%dx

    movq %r9,%rax
    movw $0x01f2,%dx
    outb %al,%dx

    movb (KernelLBALow),%al
    movw $0x1f3,%dx
    outb %al,%dx

    movb (KernelLBALow+1),%al
    movw $0x1f4,%dx
    outb %al,%dx

    movb (KernelLBALow+2),%al
    movw $0x1f5,%dx
    outb %al,%dx

    movb $0x24,%al
    movw $0x01f7,%dx
    outb %al,%dx

waitDisk:
    nop
    nop
    nop
    inb %dx,%al
    andb $0x88,%al
    cmpb $0x08,%al
    jne waitDisk
    
    movq %r9,%rax
    movl $0x100,%ecx
    mull %ecx

    xor %rcx,%rcx
    movl %eax,%ecx
    movw $0x01f0,%dx
PIO:
    inw %dx,%ax
    movw %ax,(%r10)
    addq $2,%r10
    loop PIO

    cmpq $0,%r8
    jne loadKernel

    #enter kernel
    movabsq $0xffff800000200000,%rax
    jmp *%rax

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

KernelSize: .word 0
KernelLBALow: .long 0 
KernelLBAHigh: .long 0 

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

    xor %ebx,%ebx
    movw 0x9000,%di
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
    movw $0x200,%cx
    movw 0x1c(%di),%ax
    movw 0x1e(%di),%dx
    div %cx
    cmpw $0,%dx
    je L8
    incw %ax
L8: #no ramainder
    movw %ax,(KernelSize) # serctors

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

#temp gdt for 32-bit
gdt_attribute:
    .word 255
    .quad gdt_32bit

gdt_32bit:
    .quad 0x0000000000000000
    .quad 0x00cf98000000ffff #code
    .quad 0x00cf92000000ffff #data

.code32    
protected:
    # movw $0x10,%ax
    # movw %ax,%ds
    nop
    nop
    nop
    jmp protected
    
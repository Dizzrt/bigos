.file "mbr.s"
.text
.code16

head:
    jmp mbr
.include "error.s"

DiskAddressPacket:
    .byte 0x10      #PackSize
    .byte 0         #Reserved
    .word 1         #BlockCount
    .word 0x7e00    #BufferOffset
    .word 0         #BufferSegment
    .long 8         #BlockLow
    .long 0         #BlockHigh

.global mbr
mbr:
    xor %ax,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%ss
    movw $0x500,%sp

#check Int 0x13 Extension APIs
    movb $0x41,%ah
    movw $0x55aa,%bx
    movb $0x80,%dl
    int $0x13
    cmp $0xaa55,%bx
    movb $0x30,(error_code)
    jnz error

#seek the active partition
    movw $4,%cx
    movw $Pt1,%di
seek:
    movb (%di),%bl
    cmpb $0x80,%bl
    je apFound
    addw $0x10,%di
    loop seek
    movb $0x31,(error_code)
    jmp error

#active partition found
apFound:
    movl 8(%di),%ebx #LBA of partition start
    movl %ebx,(DiskAddressPacket+8)
    movb $0x42,%ah
    movb $0x80,%dl
    mov $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    ljmp $0,$0x7e00

#fills to 440 bytes
.fill 0x1b8-(. - head),1,0

#Optional
UniqueDiskID: .long 0
#Optional
Reserved: .word 0

#Partition table entries
#debug only
Pt1:.byte 0x80 #activity
    #start CHS
    .word 0x0900
    .byte 0
    #---------
    .byte 0x0c #type FAT32
    #end CHS
    .word 0x3ffe
    .byte 0x07
    #---------
    .long 0x00000008 #LBA of partition start
    .long 0x0001fff8 # Number of sectors in partition
# Pt2:.quad 0 
#     .quad 0
# Pt3:.quad 0 
#     .quad 0
# Pt4:.quad 0 
#     .quad 0
.fill 0x1fe - (. - head) ,1,0

#BootSignature
.word 0xaa55


#debug only--------------
.fill 0xe00,1,0

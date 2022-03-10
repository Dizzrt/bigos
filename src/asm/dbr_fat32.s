.file "dbr_fat32.s"
.text
.code16

head:
    jmp dbr
    .ascii "BIGOSFAT" #identifier
    .word 0x200 #number of bytes per sector
    .byte 8 #number of sectors per cluster
    .word 8 #number of reserved sectors
    .byte 2 #number of FATs
    .word 0 #must 0 in FAT32
    .word 0 #must 0 in FAT32
    .byte 0xf8 #media descriptor
    .word 0 #must 0 in FAT32
    .word 0x003f #number of sectors per track
    .word 0x00ff #number of heads
    .long 8 #number of hidden sectors. (i.e. the LBA of the beginning of the partition.)
    #Large sector count. This field is set if there are more than 65535 sectors in the volume, 
    #resulting in a value which does not fit in the Number of Sectors entry at 0x13.
    .long 0
    .long 0 #sectors per FAT
    .word 0 #flags
    .word 0 #version
    .long 2 #the cluster number of the root directory
    .word 1 #the sector number of the FSInfo structure
    .word 0 #the sector number of the backup boot sector
    .quad 0 #reserved
    .long #reserved
    .byte 0x80 #drive number, floppy:0 ,hard disk:0x80
    .byte 0 #reserved
    .byte 0x29 #signature (must 0x28 or 0x29)
    .long 0 #volume id
    .ascii "           " #volume label
    .ascii "FAT32   " #identifiler, must "FAT32   "

DiskAddressPacket:
    .byte 0x10      #PackSize
    .byte 0         #Reserved
    .word 8         #BlockCount
    .word 0x1000    #BufferOffset
    .word 0         #BufferSegment
    .long 0         #BlockLow
    .long 0         #BlockHigh

.include "error.s"

.global dbr
dbr:
    movw $0x7e00,%di
    xor %ebx,%ebx
    movw 0x0e(%di),%bx
    movl 0x1c(%di),%eax
    addl %eax,%ebx #start position of FAT1
    movl 0x24(%di),%eax#size of FAT
    xor %cx,%cx
    mov 0x10(%di),%cl #num of FAT
AddFatSize:
    addl %eax,%ebx #start position of data area
    loop AddFatSize
    movl %ebx,(DiskAddressPacket+8)
    #read
    movb $0x42,%ah
    movb $0x80,%dl
    movw $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    #read end
    movw $0x1000,%di
    movb $0x33,(error_code)
SeekBoot:
    #search BOOT.BIN
    cmpl $0x544f4f42,(%di) # BOOT
    jne NextFile
    cmpl $0x20202020,4(%di)
    jne NextFile
    cmpw $0x4942,8(%di) #BI
    jne NextFile
    cmpb $0x4e,10(%di) #N
    jne NextFile
    jmp BootFound
NextFile:
    cmpw $0x2000,%di
    ja error # not find
    addw $0x20,%di
    jmp SeekBoot
BootFound:
    movw $0x200,%cx
    movw 0x1c(%di),%ax
    movw 0x1e(%di),%dx
    div %cx
    cmpw $0,%dx
    je NoRemainder
    incw %ax
NoRemainder:
    movw %ax,(DiskAddressPacket+2) #Block Count
    #calculate start position
    movw 0x14(%di),%ax
    shl $0x10,%eax
    movw 0x1a(%di),%ax
    subl $2,%eax
    movl $8,%ebx
    mull %ebx
    movl %edx,(DiskAddressPacket+12)
    movl %eax,(DiskAddressPacket+8)
    #read
    movb $0x42,%ah
    movb $0x80,%dl
    movw $DiskAddressPacket,%si
    int $0x13
    test %ah,%ah
    movb $0x32,(error_code)
    jnz error
    #read end
    ljmp $0,$0x1000

#fills to 440 bytes
.fill 0x1fe-(. - head),1,0
#BootSignature
.word 0xaa55

#debug only-----------------
.fill 0xe00,1,0

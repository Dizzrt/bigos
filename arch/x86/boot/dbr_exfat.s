# 
# File: dbr_exfat.s
# Created by Dizzrt on 2023/03/10.
# 
# Copyright (C) 2023 The BigOS Authors.
# Licensed under the GNU General Public License v3.0 only.
# 

.file "dbr_exfat.s"
.code16
.text

.set BASE,          0x7c00
.set DEST,          0x0600
.set OFFSET,        DEST - BASE

.set EXDBRSEG,      0x0000
.set EXDBROFFSET,   0x1000

.set STACKSEG,      0x9000
.set STACKSP,       0xfbff

.macro error msg
    movw $(\msg), %si
    call err_print
.endm

.globl _start
_start:
    # 0xeb7690 => jmp dbr
    # don't user 'jmp' directly to avoid generate other bytes sequence
    .word 0x76eb
    .byte 0x90

exfat_info:
    # for debug only
    .ascii "EXFAT   "   #0 file system name
    .fill 53, 1, 0      #8 reserved
    .quad 0x0800        #3d partition offset
    .quad 0x01f800      #45 volume length
    .long 0x0800        #4d FAT offset
    .long 0x80          #51 FAT length
    .long 0x1000        #55 cluster heap offset
    .long 0x3d00        #59 cluster count
    .long 0x05          #5d first cluster of root directory
    .long 0x8733d59e    #61 volume serial number
    .word 0x0100        #65 file system vision
    .word 0x0           #67 volume flags
    .byte 0x09          #69 bytes per sector shift
    .byte 0x03          #6a sectors per cluster shift
    .byte 0x01          #6b number of fats
    .byte 0x80          #6c drive select
    .byte 0x0           #6d percent in use
    .fill 7, 1, 0       #6e reserved

dbr:
    jmp real_dbr

DiskAddressPacket:
    .byte 0x10          # 0x00 size of packet
    .byte 0             # 0x01 reserved
    .word 8             # 0x02 number of sectors to transfer
    .word EXDBROFFSET   # 0x04 offset of transfer buffer
    .word EXDBRSEG      # 0x06 segment of transfer buffer
    .long 0             # 0x08 lower 32-bits of 48-bits strating LBA
    .long 0             # 0x0c upper 16-bits of 48-bits strating LBA

err_read: .asciz "read disk error"

print:
    movb $0x0e, %ah
print_loop:
    movb (%si), %al
    cmpb $0, %al
    je print_end
    int $0x10
    incw %si
    jmp print_loop
print_end:
    mov $0x03, %ah
    mov $0, %bx
    int $0x10
    incb %dh
    mov $0, %dl
    mov $0x02, %ah
    int $0x10
    ret

err_print:
    call print
    jmp .

real_dbr:
    # save boot drive code
    movb %dl, (0x500)

    # prepare stack
    mov $STACKSEG, %ax
    mov $STACKSP, %sp
    mov %ax, %ss

    # prepare registers
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es

    # copy self to DEST
    # [ds:si] => [es:di]
    mov $BASE, %si
    mov $DEST, %di
    mov $0x80, %cx
    rep movsd

    # load exdbr
    mov $exfat_info, %si
    mov $DiskAddressPacket, %di

    movl 0x41(%si), %eax
    movl %eax, 0x0c(%di)
    
    movl 0x3d(%si), %eax
    incl %eax
    movl %eax, 0x08(%di)

    movb $0x42, %ah
    movb (0x500), %dl
    movw $DiskAddressPacket, %si
    int $0x13
    test %ah, %ah
    jz jmp_exdbr
    error err_read

jmp_exdbr:
    movw $(exfat_info + OFFSET), %dx
    ljmp $EXDBRSEG, $EXDBROFFSET

.fill 0x1fe - (. - _start), 1, 0
.word 0xaa55

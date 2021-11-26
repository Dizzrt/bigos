.file "mbr.s"

.text
.code16
mbr:
jmpw .

FileSystemName: .ascii "EXFAT   "
MustBeZero:
    .rept 53
        .byte 0
    .endr
PartitionOffset: .quad 0
VolumeLength: .quad 0x500000
FatOffset: .long 0
FatLength: .long 0
ClusterHeapOffset: .long 0
ClusterCount: .long 0
FirstClusterOfRootDirectory: .long 0
VolumeSerialNumber: .long 0
FileSystemRevision: .word 0
VolumeFlags: .word 0
BytesPerSectorShift: .byte 0
SectorsPerClusterShift: .byte 0
NumberOfFats: .byte 0
DriveSelect: .byte 0
PercentInUse: .byte 0
Reserved_0:
    .rept 7
        .byte 1
    .endr

cur_lba: .long 1
cur_sector: .byte 2
cur_header: .byte 0
cur_cylinder: .byte 0
msg_error: .asciz "load boot error"

BootCode:
    movw $0x100,%ax
    movw %ax,%es
    movw $0,%si
read:
    movb $2,%ah
    movb $1,%al 
    movw $0,%bx

    movb $0x80,%dl
    movb cur_cylinder,%ch
    movb cur_header,%dh
    movb cur_sector,%cl

    int $0x13
    jnc read_next

    add $1,%si 
    cmpw $5,%si
    jae error
    movb $0,%ah 
    movb $0x80,%dl
    int $0x13
    jmp read
read_next:
    mov %es,%bx
    shl $4,%bx
    movl 508(%bx),%eax
    cmpl $0x456e644c,%eax
    jne continue
    movl 504(%bx),%eax
    cmpl $0x6f616400,%eax
    je enter_boot_L0

continue:
    incl cur_lba
    mov %es,%ax
    add $0x20,%ax
    mov %ax,%es

    incb cur_sector
    cmpb $18,cur_sector
    jbe read

    movb $1,cur_sector
    incb cur_header
    cmpb $1,cur_header
    je read

    movb $0,cur_header
    incb cur_cylinder
    cmpb $80,cur_cylinder
    jb read
    jmp error

enter_boot_L0:
    incl cur_lba
enter_boot_L1:
    movl cur_lba,%eax
    movl %eax,0x500 

    mov $0x1000,%ax
    jmpw *%ax

.global error
error:
    movw $msg_error,%si
print:
    movb (%si),%al
    cmpb $0,%al
    je end
    movb $0x0e,%ah
    int $0x10
    inc %si 
    jmp print
end:
    jmp .

UniqueDiskID:
    .fill 0x1b8-(. - mbr),1,0
    .long 0

Reserved_1: .word 0

PartitionTableEntries:
PT1:
    .quad 0 
    .quad 0
PT2:
    .quad 0 
    .quad 0
PT3:
    .quad 0 
    .quad 0
PT4:
    .quad 1
    .quad 1

BootSignature: .word 0xaa55

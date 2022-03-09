.file "dbr_fat32.s"
.text
.code16

head:
    jmp dbr
    .ascii "BIGOSFAT" #identifier
    .word 0x200 #number of bytes per sector
    .byte 8 #number of sectors per cluster
    .word 1 #number of reserved sectors
    .byte 2 #number of FATs
    .word 0 #must 0 in FAT32
    .word 0 #must 0 in FAT32
    .byte 0xf8 #media descriptor
    .word 0 #must 0 in FAT32
    .word 0x003f #number of sectors per track
    .word 0x00ff #number of heads
    .long 0 #number of hidden sectors. (i.e. the LBA of the beginning of the partition.)
    #Large sector count. This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13.
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

dbr:

#fills to 440 bytes
.fill 0x1fe-(. - head),1,0
#BootSignature
.word 0xaa55

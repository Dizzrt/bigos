
.file "kernel.s"

.text
.code64
#.extern _init
.extern Kernel
    
.global _start
.type _start, @function
_start:
    movq $0x4000004fff,%rsp
    #call _init
    jmp Kernel

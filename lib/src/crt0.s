.file "crt0.s"
.code64
.section .text

.extern Kernel
.extern _init
.extern _fini

.global _start
.type _start, @function
_start:
    call _init
    call Kernel
    call _fini

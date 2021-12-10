.code64
.section .bigos

.extern Kernel
.extern _init
.extern _fini

.global _start
.type _start, @function
_start:
    call _init
    call Kernel
    call _fini


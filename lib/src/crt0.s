# 
# File: crt0.s
# Created by Dizzrt on 2023/03/23.
# 
# Copyright (C) 2023 The BigOS Authors.
# Licensed under the GNU General Public License v3.0 only.
# 

.code64
.section .bigos, "ax"
.extern kernel
.extern _init
.extern _fini

.globl _start
.type _start, @function
_start:
    call _init
    call kernel
    call _fini

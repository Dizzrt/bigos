# 
# File: crtn.s
# Created by Dizzrt on 2023/03/23.
# 
# Copyright (C) 2023 The BigOS Authors.
# Licensed under the GNU General Public License v3.0 only.
# 

/* x86_64 crtn.s */
.code64
.section .init
	/* gcc will nicely put the contents of crtend.o's .init section here. */
	popq %rbp
	ret

.section .fini
	/* gcc will nicely put the contents of crtend.o's .fini section here. */
	popq %rbp
	ret

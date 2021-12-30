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

.section .endPointer
.global __endPointer
__endPointer:
	.quad .

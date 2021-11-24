	.file	"kernel.cpp"
	.text
	.p2align 4
	.globl	Kernel
	.type	Kernel, @function
Kernel:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$52, %edi
	movl	$11932, %esi
	movabsq	$ppt_init, %rax
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	call	*%rax
	movabsq	$intr_init, %rax
	call	*%rax
	movabsq	$svga_Clear, %rax
	call	*%rax
	leaq	-9(%rbp), %rdx
	movl	$105, %ecx
	movabsq	$7308905025183444585, %rax
	movabsq	$274878660624, %rdi
	movq	%rax, -10(%rbp)
	movl	$108, %eax
	movw	%ax, -2(%rbp)
	movabsq	$274878660608, %rax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L8:
	movzbl	(%rdx), %ecx
	movq	%rsi, %rax
	addq	$1, %rdx
.L3:
	movb	%cl, (%rax)
	leaq	2(%rax), %rsi
	movb	$12, 1(%rax)
	cmpq	%rdi, %rax
	jne	.L8
.L2:
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	Kernel, .-Kernel
	.ident	"GCC: (GNU) 11.2.0"

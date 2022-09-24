	.file	"simple2.c"
	.text
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	movdqa	.LC0(%rip), %xmm2
	movdqa	.LC1(%rip), %xmm4
	xorl	%eax, %eax
	movdqa	.LC2(%rip), %xmm3
.L2:
	movdqa	%xmm2, %xmm0
	addq	$32, %rax
	paddd	%xmm4, %xmm2
	cvtdq2pd	%xmm0, %xmm1
	movaps	%xmm1, b-32(%rax)
	pshufd	$238, %xmm0, %xmm1
	paddd	%xmm3, %xmm0
	cvtdq2pd	%xmm1, %xmm1
	movaps	%xmm1, b-16(%rax)
	cvtdq2pd	%xmm0, %xmm1
	pshufd	$238, %xmm0, %xmm0
	cvtdq2pd	%xmm0, %xmm0
	movaps	%xmm1, a-32(%rax)
	movaps	%xmm0, a-16(%rax)
	cmpq	$16384, %rax
	jne	.L2
	movsd	.LC4(%rip), %xmm3
	movsd	c(%rip), %xmm1
	movl	$1000000, %edx
	unpcklpd	%xmm3, %xmm3
.L3:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L4:
	movapd	a(%rax), %xmm0
	addq	$16, %rax
	mulpd	%xmm3, %xmm0
	addpd	b-16(%rax), %xmm0
	addsd	%xmm0, %xmm1
	unpckhpd	%xmm0, %xmm0
	addsd	%xmm0, %xmm1
	cmpq	$16384, %rax
	jne	.L4
	subl	$1, %edx
	jne	.L3
	movsd	%xmm1, c(%rip)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.local	c
	.comm	c,8,8
	.local	b
	.comm	b,16384,32
	.local	a
	.comm	a,16384,32
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC1:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC2:
	.long	1
	.long	1
	.long	1
	.long	1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	-611603343
	.long	1072693352
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-2)"
	.section	.note.GNU-stack,"",@progbits

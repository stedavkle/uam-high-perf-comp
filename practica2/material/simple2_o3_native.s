	.file	"simple2.c"
	.text
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L2:
	pxor	%xmm0, %xmm0
	leal	1(%rax), %edx
	cvtsi2sdl	%eax, %xmm0
	movsd	%xmm0, b(,%rax,8)
	pxor	%xmm0, %xmm0
	cvtsi2sdl	%edx, %xmm0
	movsd	%xmm0, a(,%rax,8)
	addq	$1, %rax
	cmpq	$2048, %rax
	jne	.L2
	movsd	c(%rip), %xmm1
	movsd	.LC0(%rip), %xmm2
	movl	$1000000, %edx
.L3:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L4:
	movsd	a(%rax), %xmm0
	addq	$8, %rax
	mulsd	%xmm2, %xmm0
	addsd	b-8(%rax), %xmm0
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
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	-611603343
	.long	1072693352
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-2)"
	.section	.note.GNU-stack,"",@progbits

	.file	"f_transferencia.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"-"
	.text
	.p2align 4
	.globl	fase_WB
	.type	fase_WB, @function
fase_WB:
.LFB39:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	leaq	-8(%rsp), %rsp
	.cfi_def_cfa_offset 48
	movq	config@GOTPCREL(%rip), %r12
	movl	40(%r12), %edx
	testl	%edx, %edx
	jle	.L2
	movq	RS@GOTPCREL(%rip), %rbx
	xorl	%ebp, %ebp
	leaq	.LC0(%rip), %r13
	.p2align 4
	.p2align 3
.L4:
	movl	(%rbx), %eax
	testl	%eax, %eax
	je	.L3
	cmpl	$3, 204(%rbx)
	jne	.L3
	movl	244(%rbx), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	%r13, %rdi
	call	*72(%rax)
	.p2align 4
	.p2align 3
.L3:
	addl	$1, %ebp
	addq	$248, %rbx
	cmpl	%ebp, 40(%r12)
	jg	.L4
.L2:
	leaq	8(%rsp), %rsp
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	fase_WB_alum@PLT
	.cfi_endproc
.LFE39:
	.size	fase_WB, .-fase_WB
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:

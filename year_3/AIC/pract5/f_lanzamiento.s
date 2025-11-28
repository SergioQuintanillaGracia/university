	.file	"f_lanzamiento.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"X"
.LC1:
	.string	"i"
.LC2:
	.string	"<i>I</i>"
.LC3:
	.string	"%s"
.LC4:
	.string	"f_lanzamiento.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"ERROR Issue (%s:%d): Operaci\303\263n %s (%d) no implementada\n"
	.align 8
.LC6:
	.string	"ERROR Issue (%s:%d): Instrucci\303\263n %x (%s) no implementada\n"
	.section	.rodata.str1.1
.LC7:
	.string	"I"
	.text
	.p2align 4
	.globl	fase_ISS
	.type	fase_ISS, @function
fase_ISS:
.LFB39:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-648(%rsp), %rsp
	.cfi_def_cfa_offset 704
	movq	IF_ISS_2@GOTPCREL(%rip), %rbx
	movq	%fs:40, %rax
	movq	%rax, 632(%rsp)
	xorl	%eax, %eax
	movl	192(%rbx), %r12d
	testl	%r12d, %r12d
	jne	.L181
	movl	184(%rbx), %ebp
	movl	$1001, %eax
	testl	%ebp, %ebp
	jne	.L8
	leaq	368(%rsp), %rax
	leaq	108(%rsp), %rdx
	movl	(%rbx), %esi
	leaq	80(%rsp), %rcx
	movq	136(%rbx), %rdi
	leaq	88(%rsp), %r9
	movq	%rax, 40(%rsp)
	leaq	112(%rsp), %rax
	movq	%rax, 32(%rsp)
	leaq	104(%rsp), %rax
	movq	%rax, 24(%rsp)
	leaq	100(%rsp), %rax
	movq	%rax, 16(%rsp)
	leaq	96(%rsp), %rax
	movq	%rax, 8(%rsp)
	leaq	92(%rsp), %rax
	movq	%rax, (%rsp)
	leaq	84(%rsp), %r8
	call	fn_riscv_decode@PLT
	leal	-56(%rax), %edx
	cmpl	$29, %edx
	jbe	.L3
	cmpl	$86, %eax
	jne	.L8
	.p2align 4
	.p2align 3
.L6:
	xorl	%eax, %eax
	call	fase_ISS_alum@PLT
	movl	%eax, %r14d
.L1:
	movq	632(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L267
	leaq	648(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%r14d, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L181:
	.cfi_restore_state
	movl	$220, %eax
.L8:
	movl	192(%rbx), %r11d
	movq	136(%rbx), %rcx
	movq	PC_ISS@GOTPCREL(%rip), %rdx
	testl	%r11d, %r11d
	movq	%rcx, (%rdx)
	jne	.L10
	movq	Cancelar_Activa@GOTPCREL(%rip), %rdx
	movq	presentacion@GOTPCREL(%rip), %rsi
	movl	(%rdx), %r8d
	movq	72(%rsi), %rcx
	movl	144(%rbx), %esi
	orl	188(%rbx), %r8d
	movl	%r8d, %r14d
	jne	.L268
	movq	ISSstall@GOTPCREL(%rip), %r12
	movl	(%r12), %r9d
	testl	%r9d, %r9d
	jne	.L269
	xorl	%edx, %edx
	leaq	.LC2(%rip), %rdi
	movl	%eax, 56(%rsp)
	call	*%rcx
	movq	RB_long@GOTPCREL(%rip), %rax
	movq	config@GOTPCREL(%rip), %r13
	movl	$1, (%r12)
	movl	(%rax), %eax
	cmpl	%eax, 36(%r13)
	jle	.L1
	movq	RB_fin@GOTPCREL(%rip), %r15
	movq	RB@GOTPCREL(%rip), %r9
	movl	148(%rbx), %esi
	movslq	(%r15), %rbp
	movl	184(%rbx), %r10d
	movdqu	152(%rbx), %xmm0
	leaq	0(%rbp,%rbp,2), %rcx
	movq	%rbp, %rdx
	movdqu	168(%rbx), %xmm1
	leaq	0(%rbp,%rcx,4), %rdi
	salq	$3, %rdi
	leaq	(%r9,%rdi), %rcx
	movl	%esi, 44(%rcx)
	movq	136(%rbx), %rsi
	movl	%r10d, 80(%rcx)
	movdqu	%xmm0, 48(%rcx)
	movdqu	%xmm1, 64(%rcx)
	cmpq	$-1, %rsi
	je	.L10
	movslq	56(%rsp), %rax
	cmpl	$1001, %eax
	je	.L270
	movl	80(%rsp), %edi
	cmpl	$55, %edi
	jg	.L21
	cmpl	$2, %edi
	jle	.L23
	subl	$3, %edi
	cmpl	$52, %edi
	ja	.L23
	leaq	.L38(%rip), %r10
	movslq	(%r10,%rdi,4), %rcx
	addq	%r10, %rcx
	notrack jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L38:
	.long	.L48-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L47-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L46-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L45-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L43-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L44-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L43-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L42-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L41-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L40-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L39-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L36-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L23-.L38
	.long	.L37-.L38
	.text
	.p2align 4,,7
	.p2align 3
.L3:
	movq	$-872415246, %rcx
	btq	%rdx, %rcx
	jnc	.L6
	leal	-57(%rax), %ecx
	cmpl	$29, %ecx
	ja	.L8
	movq	$-603979786, %rdx
	btq	%rcx, %rdx
	jnc	.L6
	jmp	.L8
	.p2align 4,,7
	.p2align 3
.L268:
	movl	184(%rbx), %r10d
	testl	%r10d, %r10d
	jne	.L12
	xorl	%edx, %edx
	leaq	.LC0(%rip), %rdi
	call	*%rcx
.L10:
	movl	$1, %r14d
	jmp	.L1
	.p2align 4,,7
	.p2align 3
.L12:
	movl	$1, %edx
	leaq	.LC0(%rip), %rdi
	call	*%rcx
	jmp	.L10
	.p2align 4,,7
	.p2align 3
.L269:
	xorl	%edx, %edx
	leaq	.LC1(%rip), %rdi
	call	*%rcx
	.p2align 4,,2
	jmp	.L1
.L294:
	andb	$1, %cl
	jne	.L36
.L23:
	leaq	368(%rsp), %rax
	movl	$895, %r8d
	movq	%rax, (%rsp)
	leaq	.LC4(%rip), %rcx
	movq	stderr@GOTPCREL(%rip), %rax
	leaq	.LC6(%rip), %rdx
	movl	(%rbx), %r9d
	movl	$2, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
.L43:
	movl	16(%r13), %r11d
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L74
.L271:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%ecx, %r11d
	je	.L1
.L74:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L271
	movslq	%ecx, %rcx
	imulq	$104, %rbp, %r11
	xorl	%r8d, %r8d
	xorl	%ebp, %ebp
	imulq	$248, %rcx, %rdi
	addq	%r9, %r11
	movl	%eax, 4(%r11)
	movl	$1, (%r11)
	addq	%r10, %rdi
	movl	%eax, 4(%rdi)
	imulq	$48, %rax, %rax
	movl	%r8d, 204(%rdi)
	movl	144(%rbx), %r8d
	movl	%ebp, 8(%r11)
	movl	$1, (%rdi)
	addq	instruction_list@GOTPCREL(%rip), %rax
	movl	%r8d, 100(%r11)
	movl	%edx, 228(%rdi)
	movl	%r8d, 244(%rdi)
	movl	40(%rax), %eax
	movq	%rsi, 88(%r11)
	movslq	96(%rsp), %rsi
	salq	$4, %rax
	addq	inst_types_list@GOTPCREL(%rip), %rax
	movq	%rsi, 16(%r11)
	movq	%rsi, %r8
	movl	4(%rax), %eax
	leal	-4(%rax), %r11d
	cmpl	$1, %r11d
	jbe	.L272
	subl	$6, %eax
	andl	$-3, %eax
	jne	.L78
	movslq	84(%rsp), %r11
	salq	$4, %r11
	addq	Rfp@GOTPCREL(%rip), %r11
	movl	8(%r11), %eax
	cmpl	$4096, %eax
	je	.L242
.L75:
	movslq	%eax, %r11
	imulq	$104, %r11, %r11
	addq	%r11, %r9
	cmpl	$0, 8(%r9)
	je	.L77
	movq	24(%r9), %rax
	movq	%rax, 16(%rdi)
.L76:
	movl	$4096, %eax
.L77:
	imulq	$248, %rcx, %rcx
	addq	%r10, %rcx
	testl	%r8d, %r8d
	movl	%eax, 8(%rcx)
	movslq	104(%rsp), %rax
	movl	$4096, 24(%rcx)
	movq	%rax, 32(%rcx)
	je	.L67
	movq	%rsi, %rax
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
.L67:
	movq	Excepcion_Activa@GOTPCREL(%rip), %rax
	movl	(%rax), %ebp
	movq	presentacion@GOTPCREL(%rip), %rax
	testl	%ebp, %ebp
	movq	72(%rax), %rcx
	movl	144(%rbx), %eax
	jne	.L168
	xorl	%edx, %edx
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	call	*%rcx
.L169:
	movl	(%r15), %eax
	movl	%ebp, (%r12)
	addl	$1, %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	36(%r13)
	movq	RB_long@GOTPCREL(%rip), %rax
	addl	$1, (%rax)
	movl	%edx, (%r15)
	jmp	.L10
.L47:
	movl	20(%r13), %ecx
	movl	28(%r13), %edi
	addl	16(%r13), %ecx
	addl	24(%r13), %ecx
	addl	%ecx, %edi
	cmpl	%edi, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rsi
	imulq	$248, %rsi, %rsi
	addq	%r10, %rsi
	jmp	.L69
.L273:
	addl	$1, %ecx
	addq	$248, %rsi
	cmpl	%edi, %ecx
	je	.L1
.L69:
	movl	(%rsi), %r11d
	testl	%r11d, %r11d
	jne	.L273
	testl	%ecx, %ecx
	js	.L1
	movslq	84(%rsp), %r8
	movslq	%ecx, %rcx
	imulq	$248, %rcx, %rsi
	salq	$4, %r8
	addq	Rint@GOTPCREL(%rip), %r8
	addq	%r10, %rsi
	movl	$1, (%rsi)
	movl	8(%r8), %edi
	movl	%eax, 4(%rsi)
	movl	%edx, 228(%rsi)
	cmpl	$4096, %edi
	je	.L274
	movslq	%edi, %r8
	imulq	$104, %r8, %r8
	addq	%r9, %r8
	cmpl	$0, 8(%r8)
	je	.L71
	movq	24(%r8), %rdi
	movq	%rdi, 16(%rsi)
.L72:
	movl	$4096, %edi
.L71:
	imulq	$104, %rbp, %rbp
	movslq	104(%rsp), %rsi
	imulq	$248, %rcx, %rcx
	addq	%r9, %rbp
	xorl	%r9d, %r9d
	movl	%eax, 4(%rbp)
	movslq	96(%rsp), %rax
	leaq	(%r10,%rcx), %r14
	movl	%edi, 8(%r14)
	leaq	72(%r10,%rcx), %rdi
	movl	$1, 0(%rbp)
	movq	%rax, 16(%rbp)
	salq	$4, %rax
	movq	%rsi, 64(%r14)
	addq	Rfp@GOTPCREL(%rip), %rax
	movl	%r9d, 8(%rbp)
	movl	%edx, 8(%rax)
.L254:
	leaq	4(%rbx), %r8
	leaq	.LC3(%rip), %rcx
	movl	$128, %edx
	movl	$2, %esi
	xorl	%eax, %eax
	call	__sprintf_chk@PLT
	xorl	%r9d, %r9d
	movl	%r9d, 204(%r14)
	movl	144(%rbx), %eax
	movl	%eax, 244(%r14)
	movl	%eax, 100(%rbp)
	movq	136(%rbx), %rax
	movq	%rax, 88(%rbp)
	jmp	.L67
.L36:
	movl	16(%r13), %r11d
	movslq	%eax, %rcx
	imulq	$48, %rcx, %rcx
	addq	instruction_list@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	cmpl	$1, %ecx
	je	.L98
	cmpl	$6, %ecx
	je	.L98
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L103
.L275:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L103:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L275
.L102:
	movslq	%ecx, %r11
	imulq	$104, %rbp, %rdi
	xorl	%r14d, %r14d
	imulq	$248, %r11, %rcx
	addq	%r9, %rdi
	movl	%eax, 4(%rdi)
	movl	%r14d, 8(%rdi)
	addq	%r10, %rcx
	movl	%eax, 4(%rcx)
	movslq	96(%rsp), %rax
	movl	$1, (%rdi)
	movl	$1, (%rcx)
	movq	%rax, 16(%rdi)
	movslq	84(%rsp), %rdi
	movl	%eax, 56(%rsp)
	movq	%rax, 64(%rsp)
	movq	Rint@GOTPCREL(%rip), %rax
	salq	$4, %rdi
	movl	%edx, 228(%rcx)
	addq	%rax, %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	je	.L276
	movslq	%r8d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L105
	movq	24(%rdi), %rdi
	movq	%rdi, 16(%rcx)
.L106:
	movl	$4096, %r8d
.L105:
	imulq	$248, %r11, %rcx
	leaq	(%r10,%rcx), %r14
	movslq	88(%rsp), %rcx
	movl	%r8d, 8(%r14)
	salq	$4, %rcx
	addq	%rax, %rcx
	movl	8(%rcx), %edi
	cmpl	$4096, %edi
	je	.L277
	movslq	%edi, %rcx
	imulq	$104, %rcx, %rcx
	addq	%r9, %rcx
	cmpl	$0, 8(%rcx)
	je	.L108
	movq	24(%rcx), %rcx
	movq	%rcx, 32(%r14)
.L109:
	movl	$4096, %edi
.L108:
	imulq	$248, %r11, %rcx
	cmpl	$0, 56(%rsp)
	movl	%edi, 24(%r10,%rcx)
	je	.L164
	movq	64(%rsp), %rcx
	salq	$4, %rcx
	movl	%edx, 8(%rax,%rcx)
.L164:
	imulq	$248, %r11, %rax
.L256:
	addq	%r10, %rax
.L255:
	movl	$0, 204(%rax)
	movl	144(%rbx), %edx
	movl	%edx, 244(%rax)
	imulq	$104, %rbp, %rax
	addq	%r9, %rax
	movl	%edx, 100(%rax)
	movq	%rsi, 88(%rax)
	jmp	.L67
.L39:
	movslq	%eax, %rdx
	movl	$524, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	.LC4(%rip), %rcx
	movl	$2, %esi
	leaq	4(%rdx,%rax), %r9
	leaq	.LC5(%rip), %rdx
	movq	stderr@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	jmp	.L67
.L41:
	movl	20(%r13), %ecx
	movl	32(%r13), %edi
	addl	16(%r13), %ecx
	addl	24(%r13), %ecx
	addl	28(%r13), %ecx
	addl	%ecx, %edi
	cmpl	%edi, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rsi
	imulq	$248, %rsi, %rsi
	addq	%r10, %rsi
	jmp	.L91
.L278:
	addl	$1, %ecx
	addq	$248, %rsi
	cmpl	%edi, %ecx
	je	.L1
.L91:
	movl	(%rsi), %r8d
	testl	%r8d, %r8d
	jne	.L278
	testl	%ecx, %ecx
	js	.L1
	movslq	84(%rsp), %r8
	movslq	%ecx, %rcx
	imulq	$248, %rcx, %rsi
	salq	$4, %r8
	addq	Rint@GOTPCREL(%rip), %r8
	addq	%r10, %rsi
	movl	$1, (%rsi)
	movl	8(%r8), %edi
	movl	%eax, 4(%rsi)
	cmpl	$4096, %edi
	je	.L279
	movslq	%edi, %r8
	imulq	$104, %r8, %r8
	addq	%r9, %r8
	cmpl	$0, 8(%r8)
	je	.L93
	movq	24(%r8), %rdi
	movq	%rdi, 16(%rsi)
.L94:
	movl	$4096, %edi
.L93:
	movslq	88(%rsp), %rsi
	imulq	$248, %rcx, %r8
	salq	$4, %rsi
	addq	Rfp@GOTPCREL(%rip), %rsi
	addq	%r10, %r8
	movl	%edi, 8(%r8)
	movl	8(%rsi), %edi
	cmpl	$4096, %edi
	je	.L280
	movslq	%edi, %rsi
	imulq	$104, %rsi, %rsi
	addq	%r9, %rsi
	cmpl	$0, 8(%rsi)
	je	.L96
	movq	24(%rsi), %rsi
	movq	%rsi, 32(%r8)
.L97:
	movl	$4096, %edi
.L96:
	imulq	$248, %rcx, %rsi
	xorl	%r11d, %r11d
	imulq	$104, %rbp, %rbp
	leaq	(%r10,%rsi), %r14
	movl	%edi, 24(%r14)
	movslq	104(%rsp), %rdi
	addq	%r9, %rbp
	movl	%eax, 4(%rbp)
	xorl	%eax, %eax
	movl	%r11d, 200(%r14)
	movq	%rdi, 64(%r14)
	leaq	72(%r10,%rsi), %rdi
	movl	$1, 0(%rbp)
	movq	%rcx, 16(%rbp)
	movl	%edx, 228(%r14)
	movl	%eax, 8(%rbp)
	jmp	.L254
.L42:
	movl	20(%r13), %ecx
	movl	32(%r13), %edi
	addl	16(%r13), %ecx
	addl	24(%r13), %ecx
	addl	28(%r13), %ecx
	addl	%ecx, %edi
	cmpl	%edi, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rsi
	imulq	$248, %rsi, %rsi
	addq	%r10, %rsi
	jmp	.L83
.L281:
	addl	$1, %ecx
	addq	$248, %rsi
	cmpl	%edi, %ecx
	je	.L1
.L83:
	movl	(%rsi), %r8d
	testl	%r8d, %r8d
	jne	.L281
	testl	%ecx, %ecx
	js	.L1
	movslq	%ecx, %rcx
	xorl	%r11d, %r11d
	imulq	$248, %rcx, %rsi
	addq	%r10, %rsi
	movl	%edx, 228(%rsi)
	imulq	$104, %rbp, %rdx
	movl	%eax, 4(%rsi)
	movl	$1, (%rsi)
	addq	%r9, %rdx
	movl	%eax, 4(%rdx)
	movl	%r11d, 8(%rdx)
	movl	$1, (%rdx)
	movq	%rcx, 16(%rdx)
	movslq	84(%rsp), %rdx
	movq	Rint@GOTPCREL(%rip), %rax
	salq	$4, %rdx
	addq	%rax, %rdx
	movl	8(%rdx), %edi
	cmpl	$4096, %edi
	je	.L282
	movslq	%edi, %rdx
	imulq	$104, %rdx, %rdx
	addq	%r9, %rdx
	cmpl	$0, 8(%rdx)
	je	.L85
	movq	24(%rdx), %rdx
	movq	%rdx, 16(%rsi)
.L86:
	movl	$4096, %edi
.L85:
	movslq	88(%rsp), %rdx
	imulq	$248, %rcx, %rsi
	salq	$4, %rdx
	addq	%rdx, %rax
	addq	%r10, %rsi
	movl	8(%rax), %edx
	movl	%edi, 8(%rsi)
	cmpl	$4096, %edx
	je	.L283
	movslq	%edx, %rax
	imulq	$104, %rax, %rax
	addq	%r9, %rax
	cmpl	$0, 8(%rax)
	je	.L88
	movq	24(%rax), %rax
	movq	%rax, 32(%rsi)
.L89:
	movl	$4096, %edx
.L88:
	imulq	$248, %rcx, %rcx
	movslq	104(%rsp), %rax
	xorl	%r9d, %r9d
	leaq	4(%rbx), %r8
	movl	$2, %esi
	leaq	(%r10,%rcx), %r14
	leaq	72(%r10,%rcx), %rdi
	movl	%edx, 24(%r14)
	leaq	.LC3(%rip), %rcx
	movq	%rax, 64(%r14)
	movl	$128, %edx
	movl	%r9d, 200(%r14)
	xorl	%eax, %eax
	call	__sprintf_chk@PLT
	movl	144(%rbx), %edx
	xorl	%r10d, %r10d
	movl	%r10d, 204(%r14)
	movl	%edx, 244(%r14)
.L253:
	imulq	$104, %rbp, %rax
	movq	RB@GOTPCREL(%rip), %r9
	addq	%r9, %rax
	movl	%edx, 100(%rax)
	movq	136(%rbx), %rdx
	movq	%rdx, 88(%rax)
	jmp	.L67
.L44:
	movl	16(%r13), %r11d
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L81
.L284:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L81:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L284
	movslq	%ecx, %rcx
	imulq	$104, %rbp, %r8
	xorl	%r14d, %r14d
	imulq	$248, %rcx, %rdi
	addq	%r9, %r8
	movl	%eax, 4(%r8)
	movl	$1, (%r8)
	addq	%r10, %rdi
	movl	%eax, 4(%rdi)
	movslq	96(%rsp), %rax
	movl	$1, (%rdi)
	movl	%r14d, 8(%r8)
	movl	%edx, 228(%rdi)
	movq	%rax, %r11
	movq	%rsi, 16(%rdi)
	movq	%rax, 16(%r8)
.L265:
	movl	$4096, 8(%rdi)
	testl	%r11d, %r11d
	movslq	104(%rsp), %r8
	movl	$4096, 24(%rdi)
	movq	%r8, 32(%rdi)
	je	.L175
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
.L175:
	imulq	$248, %rcx, %rcx
	leaq	(%r10,%rcx), %rax
	jmp	.L255
.L37:
	movl	16(%r13), %r11d
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L112
.L285:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L112:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L285
	movslq	%ecx, %rcx
	imulq	$104, %rbp, %r8
	xorl	%r14d, %r14d
	imulq	$248, %rcx, %rdi
	addq	%r9, %r8
	movl	%eax, 4(%r8)
	movl	$1, (%r8)
	addq	%r10, %rdi
	movl	%eax, 4(%rdi)
	movslq	96(%rsp), %rax
	movl	%r14d, 8(%r8)
	movl	$1, (%rdi)
	movq	%rax, 16(%r8)
	xorl	%r8d, %r8d
	movl	%edx, 228(%rdi)
	movq	%rax, %r11
	movq	%r8, 16(%rdi)
	jmp	.L265
.L48:
	movl	20(%r13), %ecx
	movl	28(%r13), %edi
	addl	16(%r13), %ecx
	addl	24(%r13), %ecx
	addl	%ecx, %edi
	cmpl	%ecx, %edi
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rsi
	imulq	$248, %rsi, %rsi
	addq	%r10, %rsi
	jmp	.L59
.L286:
	addl	$1, %ecx
	addq	$248, %rsi
	cmpl	%ecx, %edi
	je	.L1
.L59:
	movl	(%rsi), %r8d
	testl	%r8d, %r8d
	jne	.L286
	testl	%ecx, %ecx
	js	.L1
	movslq	%eax, %rdi
	movslq	%ecx, %rcx
	imulq	$48, %rdi, %rdi
	imulq	$248, %rcx, %rsi
	addq	instruction_list@GOTPCREL(%rip), %rdi
	addq	%r10, %rsi
	movl	$1, (%rsi)
	movl	%eax, 4(%rsi)
	movl	%edx, 228(%rsi)
	movl	40(%rdi), %edi
	salq	$4, %rdi
	addq	inst_types_list@GOTPCREL(%rip), %rdi
	movl	4(%rdi), %edi
	leal	-4(%rdi), %r8d
	cmpl	$1, %r8d
	jbe	.L287
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L64
	movslq	84(%rsp), %r8
	salq	$4, %r8
	addq	Rfp@GOTPCREL(%rip), %r8
	movl	8(%r8), %edi
	cmpl	$4096, %edi
	je	.L240
.L61:
	movslq	%edi, %r8
	imulq	$104, %r8, %r8
	addq	%r9, %r8
	cmpl	$0, 8(%r8)
	je	.L63
	movq	24(%r8), %rdi
	movq	%rdi, 16(%rsi)
.L62:
	movl	$4096, %edi
.L63:
	imulq	$248, %rcx, %rsi
	xorl	%r8d, %r8d
	addq	%r10, %rsi
	movl	%edi, 8(%rsi)
	xorl	%edi, %edi
	movq	%rdi, 32(%rsi)
	movslq	104(%rsp), %rdi
	movl	$4096, 24(%rsi)
	movq	%rdi, 64(%rsi)
	imulq	$104, %rbp, %rsi
	addq	%r9, %rsi
	movl	%eax, 4(%rsi)
	movslq	96(%rsp), %rax
	movl	$1, (%rsi)
	movl	%r8d, 8(%rsi)
	movq	%rax, 16(%rsi)
	testl	%eax, %eax
	je	.L66
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
.L66:
	imulq	$248, %rcx, %r14
	movl	$128, %edx
	leaq	4(%rbx), %r8
	leaq	.LC3(%rip), %rcx
	movl	$2, %esi
	xorl	%eax, %eax
	movq	%r10, 56(%rsp)
	leaq	72(%r10,%r14), %rdi
	call	__sprintf_chk@PLT
	movq	56(%rsp), %r10
	movl	144(%rbx), %edx
	leaq	(%r10,%r14), %r11
	xorl	%r14d, %r14d
	movl	%r14d, 204(%r11)
	movl	%edx, 244(%r11)
	jmp	.L253
.L40:
	movslq	%eax, %rdx
	movl	$518, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
.L257:
	movq	stderr@GOTPCREL(%rip), %rax
	leaq	.LC4(%rip), %rcx
	leaq	.LC5(%rip), %rdx
	movl	$2, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
.L45:
	movslq	%eax, %rdx
	movl	$342, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
.L46:
	movslq	%eax, %rdx
	movl	$336, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
	.p2align 4,,7
	.p2align 3
.L270:
	movl	144(%rbx), %eax
	testl	%r10d, %r10d
	movq	%rsi, 88(%rcx)
	movabsq	$4299262263297, %rsi
	movl	$0, 40(%rcx)
	movl	%eax, 100(%rcx)
	movq	%rsi, (%r9,%rdi)
	movl	$1, 8(%rcx)
	je	.L183
	cmpl	$32, %r10d
	jg	.L183
.L18:
	movq	Excepcion_Activa@GOTPCREL(%rip), %rdx
	movl	$1, (%rdx)
.L168:
	movl	%eax, %esi
	movl	$1, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	leaq	.LC7(%rip), %rdi
	movl	$1, %ebp
	call	*72(%rax)
	jmp	.L169
.L21:
	cmpl	$83, %edi
	jle	.L288
	subl	$87, %edi
	cmpl	$36, %edi
	.p2align 4,,2
	ja	.L23
	leaq	.L25(%rip), %r11
	movslq	(%r11,%rdi,4), %rcx
	addq	%r11, %rcx
	notrack jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L25:
	.long	.L33-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L32-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L31-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L30-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L29-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L28-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L27-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L26-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L23-.L25
	.long	.L24-.L25
	.text
.L27:
	imulq	$104, %rbp, %rdx
	xorl	%r14d, %r14d
	addq	%r9, %rdx
	testl	%r10d, %r10d
	movl	$1, (%rdx)
	movl	%eax, 4(%rdx)
	movl	%r14d, 40(%rdx)
	movl	$1, 8(%rdx)
	je	.L184
	cmpl	$32, %r10d
	jg	.L184
.L166:
	movq	Excepcion_Activa@GOTPCREL(%rip), %rax
	imulq	$104, %rbp, %rdx
	movl	$1, (%rax)
	movl	144(%rbx), %eax
	addq	%r9, %rdx
	movq	%rsi, 88(%rdx)
	movl	%eax, 100(%rdx)
	jmp	.L168
.L28:
	imulq	$104, %rbp, %rcx
	movslq	104(%rsp), %rdi
	addq	%rsi, %rdi
	addq	%r9, %rcx
	movl	%eax, 4(%rcx)
	movslq	96(%rsp), %rax
	movq	%rdi, 32(%rcx)
	leaq	4(%rsi), %rdi
	movl	$1, (%rcx)
	movq	%rax, 16(%rcx)
	testl	%eax, %eax
	movl	$1, 40(%rcx)
	movl	$1, 8(%rcx)
	movq	%rdi, 24(%rcx)
	je	.L165
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
.L165:
	imulq	$104, %rbp, %rax
	movl	144(%rbx), %edx
	addq	%r9, %rax
	movl	%edx, 100(%rax)
	movq	%rsi, 88(%rax)
	jmp	.L67
.L30:
	movl	16(%r13), %r11d
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L158
.L289:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L158:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L289
	movslq	%eax, %rdi
	movslq	%ecx, %r11
	imulq	$48, %rdi, %rdi
	imulq	$248, %r11, %rcx
	addq	instruction_list@GOTPCREL(%rip), %rdi
	addq	%r10, %rcx
	movl	$1, (%rcx)
	movl	%eax, 4(%rcx)
	movl	%edx, 228(%rcx)
	movl	40(%rdi), %edi
	salq	$4, %rdi
	addq	inst_types_list@GOTPCREL(%rip), %rdi
	movl	4(%rdi), %edi
	leal	-4(%rdi), %r8d
	cmpl	$1, %r8d
	jbe	.L290
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L162
	movslq	84(%rsp), %r8
	salq	$4, %r8
	addq	Rfp@GOTPCREL(%rip), %r8
	movl	8(%r8), %edi
	cmpl	$4096, %edi
	je	.L252
.L159:
	movslq	%edi, %r8
	imulq	$104, %r8, %r8
	addq	%r9, %r8
	cmpl	$0, 8(%r8)
	je	.L161
	movq	24(%r8), %rdi
	movq	%rdi, 16(%rcx)
.L160:
	movl	$4096, %edi
.L161:
	imulq	$248, %r11, %rcx
	xorl	%r8d, %r8d
	addq	%r10, %rcx
	movl	%edi, 8(%rcx)
	xorl	%edi, %edi
	movq	%rdi, 32(%rcx)
	movslq	104(%rsp), %rdi
	movl	$4096, 24(%rcx)
	movq	%rdi, 64(%rcx)
	imulq	$104, %rbp, %rcx
	addq	%r9, %rcx
	movl	%eax, 4(%rcx)
	movslq	96(%rsp), %rax
	movl	%r8d, 8(%rcx)
	leaq	4(%rsi), %r8
	movl	$1, (%rcx)
	movq	%rax, 16(%rcx)
	testl	%eax, %eax
	movl	$1, 40(%rcx)
	movq	%r8, 24(%rcx)
	je	.L164
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
	jmp	.L164
.L31:
	movl	16(%r13), %r11d
	testl	%r11d, %r11d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%ecx, %ecx
	movq	%r10, %rdi
	jmp	.L145
.L291:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L145:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L291
	movslq	104(%rsp), %r8
	movslq	%ecx, %rcx
	imulq	$248, %rcx, %rdi
	movq	inst_types_list@GOTPCREL(%rip), %r11
	addq	%rsi, %r8
	addq	%r10, %rdi
	movl	%edx, 228(%rdi)
	imulq	$104, %rbp, %rdx
	movl	%eax, 4(%rdi)
	movl	$1, (%rdi)
	addq	%r9, %rdx
	movl	%eax, 4(%rdx)
	imulq	$48, %rax, %rax
	movq	%r8, 32(%rdx)
	xorl	%r8d, %r8d
	movl	$1, (%rdx)
	movl	%r8d, 8(%rdx)
	addq	instruction_list@GOTPCREL(%rip), %rax
	movl	40(%rax), %r8d
	movq	%r8, %rax
	salq	$4, %rax
	movl	4(%r11,%rax), %eax
	leal	-4(%rax), %edx
	cmpl	$1, %edx
	jbe	.L292
	subl	$6, %eax
	andl	$-3, %eax
	jne	.L149
	movslq	84(%rsp), %rdx
	salq	$4, %rdx
	addq	Rfp@GOTPCREL(%rip), %rdx
	movl	8(%rdx), %eax
	cmpl	$4096, %eax
	je	.L248
.L146:
	movslq	%eax, %rdx
	imulq	$104, %rdx, %rdx
	addq	%r9, %rdx
	cmpl	$0, 8(%rdx)
	je	.L148
	movq	24(%rdx), %rax
	movq	%rax, 16(%rdi)
.L147:
	movl	$4096, %eax
.L148:
	imulq	$248, %rcx, %rdx
	addq	%r10, %rdx
	movl	%eax, 8(%rdx)
	movq	%r8, %rax
	salq	$4, %rax
	movl	8(%r11,%rax), %eax
	leal	-4(%rax), %edi
	cmpl	$1, %edi
	jbe	.L293
	subl	$6, %eax
	andl	$-3, %eax
	jne	.L155
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	Rfp@GOTPCREL(%rip), %rdi
	movl	8(%rdi), %eax
	cmpl	$4096, %eax
	je	.L250
.L152:
	movslq	%eax, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L154
	movq	24(%rdi), %rax
	movq	%rax, 32(%rdx)
.L153:
	movl	$4096, %eax
.L154:
	imulq	$248, %rcx, %rcx
	leaq	(%r10,%rcx), %rdx
	xorl	%r10d, %r10d
	movl	%eax, 24(%rdx)
	imulq	$104, %rbp, %rax
	movl	144(%rbx), %ecx
	movl	%r10d, 204(%rdx)
	movl	%ecx, 244(%rdx)
	addq	%r9, %rax
	movl	%ecx, 100(%rax)
	movq	%rsi, 88(%rax)
	jmp	.L67
.L29:
	movslq	%eax, %rdx
	movl	$804, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
.L32:
	movslq	%eax, %rdx
	movl	$721, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
.L33:
	movslq	%eax, %rdx
	movl	$716, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
.L24:
	movslq	%eax, %rdx
	movl	$889, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
.L26:
	movslq	%eax, %rdx
	movl	$883, %r8d
	imulq	$48, %rdx, %rdx
	movl	%eax, (%rsp)
	movq	instruction_list@GOTPCREL(%rip), %rax
	leaq	4(%rdx,%rax), %r9
	jmp	.L257
	.p2align 4,,7
	.p2align 3
.L183:
	movl	104(%rsp), %esi
	leaq	0(%rbp,%rbp,2), %rdx
	leaq	0(%rbp,%rdx,4), %rcx
	leal	32(%rsi), %edx
	movl	%edx, 80(%r9,%rcx,8)
	jmp	.L18
.L288:
	cmpl	$58, %edi
	jle	.L23
	leal	-59(%rdi), %ecx
	movl	$1, %r10d
	salq	%cl, %r10
	testl	$1118464, %r10d
	movq	%r10, %rcx
	jne	.L34
	cmpl	$83, %edi
	jne	.L294
	movl	16(%r13), %ecx
	movl	20(%r13), %r11d
	addl	%ecx, %r11d
	cmpl	%r11d, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rdi
	imulq	$248, %rdi, %rdi
	addq	%r10, %rdi
	jmp	.L132
.L295:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L132:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L295
	testl	%ecx, %ecx
	js	.L1
	movslq	%ecx, %rdi
	imulq	$248, %rdi, %rcx
	movq	%rdi, 56(%rsp)
	movslq	84(%rsp), %rdi
	salq	$4, %rdi
	leaq	(%r10,%rcx), %r11
	movl	100(%rsp), %ecx
	movl	$1, (%r11)
	movl	%eax, 4(%r11)
	movl	%ecx, 236(%r11)
	movq	Rfp@GOTPCREL(%rip), %rcx
	movl	%edx, 228(%r11)
	addq	%rcx, %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	je	.L296
	movslq	%r8d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L134
	movq	24(%rdi), %rdi
	movq	%rdi, 16(%r11)
.L135:
	movl	$4096, %r8d
.L134:
	imulq	$248, 56(%rsp), %rdi
	movq	inst_types_list@GOTPCREL(%rip), %r11
	leaq	(%r10,%rdi), %r14
	movslq	%eax, %rdi
	imulq	$48, %rdi, %rdi
	movl	%r8d, 8(%r14)
	addq	instruction_list@GOTPCREL(%rip), %rdi
	movl	40(%rdi), %edi
	movq	%rdi, 64(%rsp)
	salq	$4, %rdi
	movl	8(%r11,%rdi), %edi
	leal	-4(%rdi), %r8d
	cmpl	$1, %r8d
	jbe	.L297
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L140
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	%rcx, %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	je	.L246
.L137:
	movslq	%r8d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L139
	movq	24(%rdi), %rdi
	movq	%rdi, 32(%r14)
.L138:
	movl	$4096, %r8d
.L139:
	imulq	$248, 56(%rsp), %rdi
	xorl	%r14d, %r14d
	addq	%r10, %rdi
	movl	%r8d, 24(%rdi)
	movq	%r14, 48(%rdi)
	xorl	%r14d, %r14d
	movl	$4096, 40(%rdi)
	imulq	$104, %rbp, %rdi
	addq	%r9, %rdi
	movl	%eax, 4(%rdi)
	movslq	96(%rsp), %rax
	movl	$1, (%rdi)
	movl	%r14d, 8(%rdi)
	movq	%rax, 16(%rdi)
	movq	64(%rsp), %rdi
	salq	$4, %rdi
	movl	(%r11,%rdi), %edi
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L142
	salq	$4, %rax
	movl	%edx, 8(%rcx,%rax)
.L143:
	imulq	$248, 56(%rsp), %rax
	jmp	.L256
.L34:
	movl	20(%r13), %ecx
	movl	24(%r13), %r11d
	addl	16(%r13), %ecx
	addl	%ecx, %r11d
	cmpl	%r11d, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rdi
	imulq	$248, %rdi, %rdi
	addq	%r10, %rdi
	jmp	.L114
.L298:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L114:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L298
	testl	%ecx, %ecx
	js	.L1
	movq	inst_types_list@GOTPCREL(%rip), %r11
	movslq	%ecx, %rdi
	imulq	$248, %rdi, %rcx
	movq	%rdi, 64(%rsp)
	leaq	(%r10,%rcx), %r8
	movslq	%eax, %rcx
	imulq	$48, %rcx, %rcx
	movl	$1, (%r8)
	movl	%eax, 4(%r8)
	movl	%edx, 228(%r8)
	addq	instruction_list@GOTPCREL(%rip), %rcx
	movl	40(%rcx), %edi
	movq	%rdi, 72(%rsp)
	salq	$4, %rdi
	movl	4(%r11,%rdi), %ecx
	leal	-4(%rcx), %edi
	cmpl	$1, %edi
	jbe	.L299
	subl	$6, %ecx
	andl	$-3, %ecx
	jne	.L119
	movslq	84(%rsp), %rdi
	movq	Rfp@GOTPCREL(%rip), %rcx
	salq	$4, %rdi
	addq	%rcx, %rdi
	movl	8(%rdi), %r14d
	cmpl	$4096, %r14d
	je	.L300
	movslq	%r14d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L118
	movq	24(%rdi), %rdi
	movq	%rdi, 16(%r8)
.L117:
	movl	$4096, %r14d
.L118:
	imulq	$248, 64(%rsp), %rdi
	addq	%r10, %rdi
	movl	%r14d, 8(%rdi)
	movq	%rdi, 56(%rsp)
	movq	72(%rsp), %rdi
	salq	$4, %rdi
	movl	8(%r11,%rdi), %edi
	leal	-4(%rdi), %r8d
	cmpl	$1, %r8d
	jbe	.L301
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L125
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	%rcx, %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	je	.L244
.L122:
	movslq	%r8d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L124
	movq	24(%rdi), %rdi
	movq	56(%rsp), %r8
	movq	%rdi, 32(%r8)
.L123:
	movl	$4096, %r8d
.L124:
	imulq	$248, 64(%rsp), %rdi
	leaq	(%r10,%rdi), %r14
	movq	72(%rsp), %rdi
	movl	%r8d, 24(%r14)
	salq	$4, %rdi
	movl	12(%r11,%rdi), %edi
	subl	$6, %edi
	andl	$-3, %edi
	jne	.L127
	movslq	92(%rsp), %rdi
	salq	$4, %rdi
	addq	%rcx, %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	je	.L302
	movslq	%r8d, %rdi
	imulq	$104, %rdi, %rdi
	addq	%r9, %rdi
	cmpl	$0, 8(%rdi)
	je	.L130
	movq	24(%rdi), %rdi
	movq	%rdi, 48(%r14)
.L129:
	movl	$4096, %r8d
.L130:
	imulq	$248, 64(%rsp), %rdi
	addq	%r10, %rdi
	xorl	%r10d, %r10d
	movl	%r8d, 40(%rdi)
	imulq	$104, %rbp, %r8
	movl	%r10d, 204(%rdi)
	addq	%r9, %r8
	xorl	%r9d, %r9d
	movl	%eax, 4(%r8)
	movslq	96(%rsp), %rax
	movl	$1, (%r8)
	movl	%r9d, 8(%r8)
	movq	%rax, 16(%r8)
	salq	$4, %rax
	movl	%edx, 8(%rcx,%rax)
	movl	144(%rbx), %eax
	movq	%rsi, 88(%r8)
	movl	%eax, 244(%rdi)
	movl	%eax, 100(%r8)
	jmp	.L67
.L267:
	call	__stack_chk_fail@PLT
.L98:
	movl	20(%r13), %ecx
	addl	%r11d, %ecx
	movl	24(%r13), %r11d
	addl	%ecx, %r11d
	cmpl	%r11d, %ecx
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%ecx, %rdi
	imulq	$248, %rdi, %rdi
	addq	%r10, %rdi
	jmp	.L101
.L303:
	addl	$1, %ecx
	addq	$248, %rdi
	cmpl	%r11d, %ecx
	je	.L1
.L101:
	movl	(%rdi), %r8d
	testl	%r8d, %r8d
	jne	.L303
	testl	%ecx, %ecx
	jns	.L102
	.p2align 4,,3
	jmp	.L1
.L184:
	imulq	$104, %rbp, %rax
	movl	$255, 80(%r9,%rax)
	jmp	.L166
.L276:
	movq	(%rdi), %rdi
	movq	%rdi, 16(%rcx)
	jmp	.L106
.L78:
	xorl	%r9d, %r9d
	movq	%r9, 16(%rdi)
	jmp	.L76
.L142:
	testl	%eax, %eax
	je	.L143
	salq	$4, %rax
	addq	Rint@GOTPCREL(%rip), %rax
	movl	%edx, 8(%rax)
	jmp	.L143
.L277:
	movq	(%rcx), %rcx
	movq	%rcx, 32(%r14)
	jmp	.L109
.L274:
	movq	(%r8), %rdi
	movq	%rdi, 16(%rsi)
	jmp	.L72
.L297:
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	Rint@GOTPCREL(%rip), %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	jne	.L137
.L246:
	movq	(%rdi), %rdi
	movq	%rdi, 32(%r14)
	jmp	.L138
.L296:
	movq	(%rdi), %rdi
	movq	%rdi, 16(%r11)
	jmp	.L135
.L155:
	xorl	%r11d, %r11d
	movq	%r11, 32(%rdx)
	jmp	.L153
.L293:
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	Rint@GOTPCREL(%rip), %rdi
	movl	8(%rdi), %eax
	cmpl	$4096, %eax
	jne	.L152
.L250:
	movq	(%rdi), %rax
	movq	%rax, 32(%rdx)
	jmp	.L153
.L149:
	xorl	%r14d, %r14d
	movq	%r14, 16(%rdi)
	jmp	.L147
.L292:
	movslq	84(%rsp), %rdx
	salq	$4, %rdx
	addq	Rint@GOTPCREL(%rip), %rdx
	movl	8(%rdx), %eax
	cmpl	$4096, %eax
	jne	.L146
.L248:
	movq	(%rdx), %rax
	movq	%rax, 16(%rdi)
	jmp	.L147
.L290:
	movslq	84(%rsp), %r8
	salq	$4, %r8
	addq	Rint@GOTPCREL(%rip), %r8
	movl	8(%r8), %edi
	cmpl	$4096, %edi
	jne	.L159
.L252:
	movq	(%r8), %rdi
	movq	%rdi, 16(%rcx)
	jmp	.L160
.L162:
	xorl	%r14d, %r14d
	movq	%r14, 16(%rcx)
	jmp	.L160
.L127:
	xorl	%r11d, %r11d
	movq	%r11, 48(%r14)
	jmp	.L129
.L301:
	movslq	88(%rsp), %rdi
	salq	$4, %rdi
	addq	Rint@GOTPCREL(%rip), %rdi
	movl	8(%rdi), %r8d
	cmpl	$4096, %r8d
	jne	.L122
.L244:
	movq	(%rdi), %rdi
	movq	56(%rsp), %r8
	movq	%rdi, 32(%r8)
	jmp	.L123
.L299:
	movslq	84(%rsp), %rcx
	salq	$4, %rcx
	addq	Rint@GOTPCREL(%rip), %rcx
	movl	8(%rcx), %r14d
	cmpl	$4096, %r14d
	je	.L304
	movslq	%r14d, %rcx
	imulq	$104, %rcx, %rcx
	addq	%r9, %rcx
	cmpl	$0, 8(%rcx)
	jne	.L233
	movq	Rfp@GOTPCREL(%rip), %rcx
	jmp	.L118
.L279:
	movq	(%r8), %rdi
	movq	%rdi, 16(%rsi)
	jmp	.L94
.L272:
	movslq	84(%rsp), %r11
	salq	$4, %r11
	addq	Rint@GOTPCREL(%rip), %r11
	movl	8(%r11), %eax
	cmpl	$4096, %eax
	jne	.L75
.L242:
	movq	(%r11), %rax
	movq	%rax, 16(%rdi)
	jmp	.L76
.L283:
	movq	(%rax), %rax
	movq	%rax, 32(%rsi)
	jmp	.L89
.L282:
	movq	(%rdx), %rdx
	movq	%rdx, 16(%rsi)
	jmp	.L86
.L287:
	movslq	84(%rsp), %r8
	salq	$4, %r8
	addq	Rint@GOTPCREL(%rip), %r8
	movl	8(%r8), %edi
	cmpl	$4096, %edi
	jne	.L61
.L240:
	movq	(%r8), %rdi
	movq	%rdi, 16(%rsi)
	jmp	.L62
.L280:
	movq	(%rsi), %rsi
	movq	%rsi, 32(%r8)
	jmp	.L97
.L64:
	xorl	%r11d, %r11d
	movq	%r11, 16(%rsi)
	jmp	.L62
.L233:
	movq	24(%rcx), %rcx
	movq	%rcx, 16(%r8)
	movq	Rfp@GOTPCREL(%rip), %rcx
	jmp	.L117
.L304:
	movq	(%rcx), %rcx
	movq	%rcx, 16(%r8)
	movq	Rfp@GOTPCREL(%rip), %rcx
	jmp	.L117
.L140:
	xorl	%edi, %edi
	movq	%rdi, 32(%r14)
	jmp	.L138
.L125:
	movq	56(%rsp), %rdi
	xorl	%r14d, %r14d
	movq	%r14, 32(%rdi)
	jmp	.L123
.L119:
	xorl	%ecx, %ecx
	movq	%rcx, 16(%r8)
	movq	Rfp@GOTPCREL(%rip), %rcx
	jmp	.L117
.L302:
	movq	(%rdi), %rdi
	movq	%rdi, 48(%r14)
	jmp	.L129
.L300:
	movq	(%rdi), %rdi
	movq	%rdi, 16(%r8)
	jmp	.L117
	.cfi_endproc
.LFE39:
	.size	fase_ISS, .-fase_ISS
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

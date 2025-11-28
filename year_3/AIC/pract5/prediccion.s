	.file	"prediccion.c"
	.text
	.p2align 4
	.type	cambia_estado_histeresis, @function
cambia_estado_histeresis:
.LFB54:
	.cfi_startproc
	movl	$1, %eax
	movl	%edi, %r9d
	leal	-1(%rdx), %ecx
	movl	%eax, %r8d
	sall	%cl, %r8d
	testl	%r9d, %r9d
	movl	(%rsi), %edi
	je	.L2
	movl	%edx, %ecx
	subl	$1, %r8d
	sall	%cl, %eax
	subl	$1, %eax
	cmpl	%edi, %r8d
	je	.L7
	cmpl	%edi, %eax
	jle	.L1
	addl	$1, %edi
	movl	%edi, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L2:
	cmpl	%r8d, %edi
	je	.L8
	testl	%edi, %edi
	.p2align 4,,2
	jle	.L1
	subl	$1, %edi
	movl	%edi, (%rsi)
.L1:
	.p2align 4,,1
	ret
	.p2align 4,,7
	.p2align 3
.L8:
	movl	$0, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L7:
	movl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE54:
	.size	cambia_estado_histeresis, .-cambia_estado_histeresis
	.p2align 4
	.globl	inicializa_prediccion
	.type	inicializa_prediccion, @function
inicializa_prediccion:
.LFB55:
	.cfi_startproc
	endbr64
	movq	config@GOTPCREL(%rip), %rax
	movslq	104(%rax), %rax
	testl	%eax, %eax
	jle	.L12
	movq	%rax, %r9
	movl	$24, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	salq	$6, %r9
	addq	%rax, %r9
	xorl	%eax, %eax
	salq	$5, %r9
	leaq	24(%rdx), %rdx
	addq	$24, %r9
	.p2align 4
	.p2align 3
.L11:
	movq	%rdx, %rdi
	leaq	1024(%rdx), %r8
	movl	$128, %ecx
	addq	$2080, %rsi
	movq	$-1, -24(%rdx)
	addq	$2080, %rdx
	movl	$0, -2096(%rdx)
	rep stosq
	movl	$128, %ecx
	movq	%r8, %rdi
	rep stosq
	movq	$0, -2088(%rdx)
	movl	$0, -32(%rdx)
	movl	$0, -28(%rdx)
	cmpq	%r9, %rsi
	jne	.L11
.L12:
	movq	BHSR1@GOTPCREL(%rip), %rax
	pxor	%xmm0, %xmm0
	movdqu	%xmm0, (%rax)
	movdqu	%xmm0, 16(%rax)
	movdqu	%xmm0, 32(%rax)
	movdqu	%xmm0, 48(%rax)
	movq	BHSR2@GOTPCREL(%rip), %rax
	movdqu	%xmm0, (%rax)
	movdqu	%xmm0, 16(%rax)
	movdqu	%xmm0, 32(%rax)
	movdqu	%xmm0, 48(%rax)
	ret
	.cfi_endproc
.LFE55:
	.size	inicializa_prediccion, .-inicializa_prediccion
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"%u %ld %u %ld\n"
.LC2:
	.string	"prediccion.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"Error: %s; funci\303\263n:%s; l\303\255nea:%d\n"
	.section	.rodata.str1.1
.LC4:
	.string	"Final de la traza de saltos\n"
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"ERROR (%s:%d): Predictor no implementado\n"
	.text
	.p2align 4
	.globl	obtener_prediccion
	.type	obtener_prediccion, @function
obtener_prediccion:
.LFB56:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdi, %rax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rcx, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-40(%rsp), %rsp
	.cfi_def_cfa_offset 96
	movq	config@GOTPCREL(%rip), %rbx
	movq	$0, (%rcx)
	movl	$0, (%rdx)
	movq	%rdi, %rdx
	movslq	128(%rbx), %rcx
	sarq	$63, %rdx
	movq	BHSR1@GOTPCREL(%rip), %r15
	idivq	%rcx
	movslq	136(%rbx), %rcx
	movl	(%r15,%rdx,4), %eax
	movq	%rdi, %rdx
	sarq	$63, %rdx
	movl	%eax, 8(%r8)
	movq	%rdi, %rax
	idivq	%rcx
	movq	BHSR2@GOTPCREL(%rip), %rax
	movl	(%rax,%rdx,4), %eax
	movq	$0, (%r8)
	movl	%eax, 12(%r8)
	movq	.LC0(%rip), %rax
	movq	%rax, 16(%r8)
	movl	108(%rbx), %eax
	cmpl	$11, %eax
	je	.L17
	leal	-9(%rax), %edx
	movq	%rdi, %rbp
	cmpl	$1, %edx
	movl	%esi, %r11d
	jbe	.L18
.L21:
	movl	104(%rbx), %ecx
	testl	%ecx, %ecx
	jle	.L17
	movq	BTB@GOTPCREL(%rip), %rsi
	xorl	%r13d, %r13d
	xorl	%edi, %edi
	leaq	.L52(%rip), %r9
	jmp	.L75
	.p2align 4,,7
	.p2align 3
.L49:
	addl	$1, %edi
	addq	$2080, %rsi
	cmpl	%edi, 104(%rbx)
	jle	.L15
.L75:
	cmpq	%rbp, (%rsi)
	jne	.L49
	movq	estat@GOTPCREL(%rip), %rax
	movl	%edi, 20(%r8)
	movl	%r11d, 2076(%rsi)
	addq	$1, 48(%rax)
	cmpl	$8, 108(%rbx)
	ja	.L50
	movl	108(%rbx), %eax
	movslq	(%r9,%rax,4), %rax
	addq	%r9, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L52:
	.long	.L56-.L52
	.long	.L56-.L52
	.long	.L55-.L52
	.long	.L55-.L52
	.long	.L55-.L52
	.long	.L55-.L52
	.long	.L53-.L52
	.long	.L51-.L52
	.long	.L51-.L52
	.text
	.p2align 4,,7
	.p2align 3
.L55:
	movl	112(%rbx), %ecx
	testl	%ecx, %ecx
	je	.L59
	movslq	128(%rbx), %r10
	movq	%rbp, %rdx
	movq	%rbp, %rax
	subl	$1, %ecx
	sarq	$63, %rdx
	idivq	%r10
	movslq	(%r15,%rdx,4), %r10
	movslq	%edi, %rdx
	movq	%rdx, %rax
	salq	$6, %rax
	addq	%rdx, %rax
	leaq	4(%r10,%rax,8), %rdx
	movl	$1, %eax
	sall	%cl, %eax
	movq	BTB@GOTPCREL(%rip), %rcx
	cmpl	%eax, 8(%rcx,%rdx,4)
	jge	.L115
	.p2align 4
	.p2align 3
.L59:
	movl	$0, (%r12)
	xorl	%eax, %eax
.L60:
	addl	$1, %edi
	addq	$2080, %rsi
	cmpl	%edi, 104(%rbx)
	movl	$1, %r13d
	movl	%eax, (%r8)
	movl	$0, 4(%r8)
	movl	$0, 16(%r8)
	jg	.L75
.L15:
	leaq	40(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%r13d, %eax
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
.L56:
	.cfi_restore_state
	movl	120(%rbx), %ecx
	movl	$1, %r13d
	movl	112(%rbx), %eax
	subl	$1, %ecx
	sall	%cl, %r13d
	testl	%eax, %eax
	jne	.L118
	movq	$0, (%r8)
	cmpl	%r13d, 2072(%rsi)
	jge	.L108
	movl	$0, (%r12)
.L73:
	movl	$0, 16(%r8)
	movl	$1, %r13d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L51:
	movl	8(%rsi), %eax
	subl	$2, %eax
	cmpl	$1, %eax
	ja	.L59
.L115:
	movq	16(%rsi), %rax
	movl	$1, (%r12)
	movq	%rax, (%r14)
	movl	$1, %eax
	jmp	.L60
	.p2align 4,,7
	.p2align 3
.L53:
	cmpl	$3, 8(%rsi)
	jne	.L59
	jmp	.L115
	.p2align 4,,7
	.p2align 3
.L118:
	leal	-1(%rax), %ecx
	movl	$1, %r10d
	sall	%cl, %r10d
	movq	%rbp, %rdx
	movslq	128(%rbx), %rcx
	movq	%rbp, %rax
	sarq	$63, %rdx
	idivq	%rcx
	movslq	%edi, %rcx
	movq	%rcx, %rax
	salq	$6, %rax
	addq	%rcx, %rax
	movslq	(%r15,%rdx,4), %rdx
	leaq	4(%rdx,%rax,8), %rax
	movq	BTB@GOTPCREL(%rip), %rdx
	cmpl	%r10d, 8(%rdx,%rax,4)
	jge	.L119
	movl	$0, (%rsp)
	movl	$0, 20(%rsp)
.L69:
	movslq	136(%rbx), %rax
	movq	%rax, %rdx
	movq	%rbp, %rax
	movq	%rdx, 8(%rsp)
	movq	%rbp, %rdx
	sarq	$63, %rdx
	idivq	8(%rsp)
	movq	BHSR2@GOTPCREL(%rip), %rax
	movslq	(%rax,%rdx,4), %rdx
	movq	%rcx, %rax
	salq	$6, %rax
	addq	%rcx, %rax
	movq	BTB@GOTPCREL(%rip), %rcx
	leaq	260(%rdx,%rax,8), %rax
	cmpl	8(%rcx,%rax,4), %r10d
	jg	.L70
	movd	(%rsp), %xmm0
	movl	$1, %edx
	movq	16(%rsi), %rax
	movd	%edx, %xmm1
	punpckldq	%xmm1, %xmm0
	movq	%xmm0, (%r8)
	cmpl	%r13d, 2072(%rsi)
	jl	.L80
	movl	$1, (%r12)
	movq	%rax, (%r14)
	jmp	.L79
	.p2align 4,,7
	.p2align 3
.L18:
	movq	fase_ejecucion_pred@GOTPCREL(%rip), %rdx
	movl	(%rdx), %r13d
	testl	%r13d, %r13d
	je	.L17
	cmpl	$1, %r13d
	jne	.L21
	movq	ciclo1@GOTPCREL(%rip), %rcx
	movq	Ciclo@GOTPCREL(%rip), %rdx
	movl	(%rcx), %ecx
	cmpl	%ecx, (%rdx)
	je	.L120
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	(%rax), %r15d
	testl	%r15d, %r15d
	jne	.L121
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$1, (%r10)
	je	.L107
	movq	PC1@GOTPCREL(%rip), %rcx
.L32:
	cmpq	%rbp, (%rcx)
	jne	.L33
	movq	$-1, (%rcx)
	cmpl	$9, 108(%rbx)
	je	.L34
	movl	104(%rbx), %edx
	testl	%edx, %edx
	jle	.L36
	movq	BTB@GOTPCREL(%rip), %rax
	movslq	%edx, %rcx
	movq	%rcx, %rdx
	salq	$6, %rdx
	addq	%rcx, %rdx
	salq	$5, %rdx
	addq	%rax, %rdx
	.p2align 4
	.p2align 3
.L40:
	cmpq	%rbp, (%rax)
	jne	.L39
	movl	%r11d, 2076(%rax)
	movl	$1, %r15d
.L39:
	addq	$2080, %rax
	cmpq	%rdx, %rax
	jne	.L40
	testl	%r15d, %r15d
	je	.L36
.L34:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 48(%rax)
	movq	cond1@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	testl	%edx, %edx
	jne	.L37
	movl	$0, (%r12)
.L38:
	movl	$1, (%r10)
	jmp	.L15
	.p2align 4,,7
	.p2align 3
.L70:
	movd	(%rsp), %xmm0
	movq	%xmm0, (%r8)
	cmpl	%r13d, 2072(%rsi)
	jl	.L80
	.p2align 4
	.p2align 3
.L108:
	movl	$0, (%r12)
.L79:
	movl	$1, 16(%r8)
	movl	$1, %r13d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L119:
	movq	16(%rsi), %rax
	movl	$1, (%rsp)
	movl	$1, 20(%rsp)
	movq	%rax, 24(%rsp)
	jmp	.L69
	.p2align 4,,7
	.p2align 3
.L80:
	movl	20(%rsp), %eax
	movl	%eax, (%r12)
	testl	%eax, %eax
	je	.L73
	movq	24(%rsp), %rax
	movq	%rax, (%r14)
	jmp	.L73
.L107:
	movq	f_trace@GOTPCREL(%rip), %rax
	movq	PC1@GOTPCREL(%rip), %rcx
	movq	dest1@GOTPCREL(%rip), %r9
	movq	cond1@GOTPCREL(%rip), %r8
	movq	orden1@GOTPCREL(%rip), %rdx
	movq	(%rax), %rdi
	xorl	%eax, %eax
	movl	%esi, 8(%rsp)
	leaq	.LC1(%rip), %rsi
	movq	%rcx, (%rsp)
	call	__isoc99_fscanf@PLT
	movq	orden1@GOTPCREL(%rip), %rax
	movq	(%rsp), %rcx
	movl	8(%rsp), %r11d
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$-2, (%rax)
	jne	.L32
	movq	stderr@GOTPCREL(%rip), %rbx
	movl	$265, %r9d
	leaq	__func__.0(%rip), %r8
	leaq	.LC2(%rip), %rcx
	leaq	.LC3(%rip), %rdx
	movl	$2, %esi
	movq	(%rbx), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movq	(%rbx), %rcx
	movl	$28, %edx
	movl	$1, %esi
	leaq	.LC4(%rip), %rdi
	call	fwrite@PLT
	call	abort@PLT
	.p2align 4,,7
	.p2align 3
.L121:
	movslq	104(%rbx), %rcx
	testl	%ecx, %ecx
	jle	.L17
	movq	BTB@GOTPCREL(%rip), %rax
	movq	%rcx, %rdx
	salq	$6, %rdx
	addq	%rcx, %rdx
	xorl	%ecx, %ecx
	salq	$5, %rdx
	addq	%rax, %rdx
	.p2align 4
	.p2align 3
.L31:
	cmpq	%rbp, (%rax)
	jne	.L30
	movl	%r11d, 2076(%rax)
	movl	$1, %ecx
.L30:
	addq	$2080, %rax
	cmpq	%rdx, %rax
	jne	.L31
.L113:
	testl	%ecx, %ecx
	je	.L17
.L111:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 48(%rax)
	.p2align 4
	.p2align 3
.L17:
	xorl	%r13d, %r13d
	jmp	.L15
.L33:
	movq	IF_ISS_1@GOTPCREL(%rip), %rax
	movslq	132(%rax), %rax
	leaq	(%rax,%rax,2), %rax
	salq	$4, %rax
	addq	instruction_list@GOTPCREL(%rip), %rax
	movzbl	44(%rax), %eax
	addl	$29, %eax
	andl	$127, %eax
	cmpb	$12, %al
	ja	.L43
	movl	$4113, %edx
	btq	%rax, %rdx
	jnc	.L43
	cmpl	$9, 108(%rbx)
	je	.L114
	movslq	104(%rbx), %rcx
	testl	%ecx, %ecx
	jle	.L43
	movq	BTB@GOTPCREL(%rip), %rax
	movq	%rcx, %rdx
	salq	$6, %rdx
	addq	%rcx, %rdx
	salq	$5, %rdx
	addq	%rax, %rdx
.L47:
	cmpq	%rbp, (%rax)
	jne	.L46
	movl	%r11d, 2076(%rax)
	movl	$1, %r15d
.L46:
	addq	$2080, %rax
	cmpq	%rdx, %rax
	jne	.L47
	testl	%r15d, %r15d
	je	.L43
.L114:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 48(%rax)
.L43:
	movl	$0, (%r10)
	jmp	.L17
.L120:
	movq	IF_ISS_1@GOTPCREL(%rip), %rdx
	movslq	132(%rdx), %rdx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$4, %rdx
	addq	instruction_list@GOTPCREL(%rip), %rdx
	movzbl	44(%rdx), %edx
	addl	$29, %edx
	andl	$127, %edx
	cmpb	$12, %dl
	ja	.L17
	movl	$4113, %ecx
	btq	%rdx, %rcx
	jnc	.L17
	cmpl	$9, %eax
	je	.L111
	movslq	104(%rbx), %rcx
	testl	%ecx, %ecx
	jle	.L17
	movq	BTB@GOTPCREL(%rip), %rax
	movq	%rcx, %rdx
	salq	$6, %rdx
	addq	%rcx, %rdx
	xorl	%ecx, %ecx
	salq	$5, %rdx
	addq	%rax, %rdx
.L28:
	cmpq	%rbp, (%rax)
	jne	.L27
	movl	%r11d, 2076(%rax)
	movl	$1, %ecx
.L27:
	addq	$2080, %rax
	cmpq	%rax, %rdx
	jne	.L28
	jmp	.L113
.L36:
	movq	cond1@GOTPCREL(%rip), %rax
	movl	$0, (%r12)
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L42
	movq	orden_clarividente@GOTPCREL(%rip), %rax
	movl	%r11d, (%rax)
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	$1, (%rax)
.L42:
	movl	$1, (%r10)
	jmp	.L17
.L37:
	movq	dest1@GOTPCREL(%rip), %rax
	movl	$1, (%r12)
	movq	ciclo1@GOTPCREL(%rip), %rdx
	movq	(%rax), %rax
	movq	%rax, (%r14)
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	jmp	.L38
.L50:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$480, %r8d
	leaq	.LC2(%rip), %rcx
	leaq	.LC5(%rip), %rdx
	movl	$2, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE56:
	.size	obtener_prediccion, .-obtener_prediccion
	.section	.rodata.str1.1
.LC6:
	.string	"%u %ld %d %ld\n"
	.text
	.p2align 4
	.globl	actualizar_prediccion
	.type	actualizar_prediccion, @function
actualizar_prediccion:
.LFB57:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	%ecx, %r9d
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-72(%rsp), %rsp
	.cfi_def_cfa_offset 128
	movq	config@GOTPCREL(%rip), %rax
	movl	%edi, 40(%rsp)
	movl	%edx, 44(%rsp)
	movl	108(%rax), %r14d
	movq	%r8, 24(%rsp)
	leal	-9(%r14), %eax
	cmpl	$1, %eax
	jbe	.L200
.L123:
	movl	%r14d, %eax
	andl	$-3, %eax
	cmpl	$9, %eax
	je	.L122
	movq	config@GOTPCREL(%rip), %rax
	movl	104(%rax), %r13d
	testl	%r13d, %r13d
	jle	.L171
	movq	BTB@GOTPCREL(%rip), %rax
	xorl	%ebx, %ebx
	movq	$-1, %r15
	movl	$-1, %ebp
	xorl	%ecx, %ecx
	xorl	%r11d, %r11d
	movq	%rax, 32(%rsp)
	movq	%rax, %r10
	leaq	.L130(%rip), %rax
	jmp	.L164
	.p2align 4,,7
	.p2align 3
.L127:
	movl	2076(%r10), %edx
	cmpl	%ebp, %edx
	jnb	.L163
	movslq	%r11d, %r15
	movl	%edx, %ebp
.L163:
	addl	$1, %r11d
	addq	$2080, %r10
	addq	$520, %rbx
	cmpl	%r11d, %r13d
	je	.L201
.L164:
	cmpq	%r12, (%r10)
	jne	.L127
	movq	24(%rsp), %rdi
	cmpl	$10, %r14d
	movq	%rdi, 16(%r10)
	ja	.L128
	movl	%r14d, %edx
	movslq	(%rax,%rdx,4), %rdx
	addq	%rax, %rdx
	notrack jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L130:
	.long	.L137-.L130
	.long	.L137-.L130
	.long	.L136-.L130
	.long	.L135-.L130
	.long	.L136-.L130
	.long	.L135-.L130
	.long	.L129-.L130
	.long	.L132-.L130
	.long	.L131-.L130
	.long	.L128-.L130
	.long	.L129-.L130
	.text
	.p2align 4,,7
	.p2align 3
.L135:
	movslq	40(%rsp), %rcx
	movq	RB@GOTPCREL(%rip), %rdi
	movq	config@GOTPCREL(%rip), %rax
	movl	%r9d, 48(%rsp)
	leaq	(%rcx,%rcx,2), %rsi
	leaq	(%rcx,%rsi,4), %rcx
	movl	112(%rax), %edx
	leaq	(%rdi,%rcx,8), %rcx
	movslq	56(%rcx), %rcx
	movl	%r9d, %edi
	movq	32(%rsp), %rax
	leaq	6(%rbx,%rcx), %rcx
	leaq	(%rax,%rcx,4), %rsi
	call	cambia_estado_histeresis
	movl	48(%rsp), %r9d
	leaq	.L130(%rip), %rax
.L139:
	movl	$1, %ecx
	jmp	.L127
	.p2align 4,,7
	.p2align 3
.L136:
	movq	config@GOTPCREL(%rip), %rsi
	movslq	40(%rsp), %rdx
	movq	32(%rsp), %r8
	movl	112(%rsi), %ecx
	leaq	(%rdx,%rdx,2), %rsi
	leaq	(%rdx,%rsi,4), %rdx
	movq	RB@GOTPCREL(%rip), %rsi
	leaq	(%rsi,%rdx,8), %rdx
	movslq	%r11d, %rsi
	movslq	56(%rdx), %rdi
	movq	%rsi, %rdx
	salq	$6, %rdx
	addq	%rsi, %rdx
	testl	%r9d, %r9d
	leaq	4(%rdi,%rdx,8), %rdi
	movl	8(%r8,%rdi,4), %edx
	je	.L156
	movl	$1, %esi
	sall	%cl, %esi
	subl	$1, %esi
	cmpl	%edx, %esi
	jle	.L139
	addl	$1, %edx
	movl	%edx, 8(%r8,%rdi,4)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L201:
	testl	%ecx, %ecx
	je	.L126
.L122:
	leaq	72(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
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
.L129:
	.cfi_restore_state
	movl	%r9d, %edi
	negl	%edi
	sbbl	%edx, %edx
	andl	$3, %edx
	movl	%edx, 8(%r10)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L137:
	movslq	40(%rsp), %rsi
	movq	RB@GOTPCREL(%rip), %rdx
	leaq	(%rsi,%rsi,2), %rcx
	leaq	(%rsi,%rcx,4), %rcx
	leaq	(%rdx,%rcx,8), %rcx
	movl	48(%rcx), %edi
	movl	52(%rcx), %ecx
	cmpl	%edi, %r9d
	jne	.L190
	cmpl	%ecx, %r9d
	je	.L157
	movl	2072(%r10), %ecx
	testl	%ecx, %ecx
	jle	.L158
	subl	$1, %ecx
	movl	%ecx, 2072(%r10)
.L158:
	movq	config@GOTPCREL(%rip), %rdi
	movl	112(%rdi), %ecx
	leaq	(%rsi,%rsi,2), %rdi
	leaq	(%rsi,%rdi,4), %rsi
	leaq	(%rdx,%rsi,8), %r8
	movq	config@GOTPCREL(%rip), %rsi
	movslq	56(%r8), %rdx
	cmpb	$104, 116(%rsi)
	je	.L202
	movl	60(%r8), %esi
	testl	%r9d, %r9d
	movl	%esi, 48(%rsp)
	movslq	%r11d, %rsi
	je	.L160
	movq	32(%rsp), %r8
	movq	%rsi, %rdi
	salq	$6, %rdi
	addq	%rsi, %rdi
	leaq	4(%rdx,%rdi,8), %rdx
	movl	8(%r8,%rdx,4), %edi
	movl	%edi, 52(%rsp)
	movl	$1, %edi
	sall	%cl, %edi
	leal	-1(%rdi), %ecx
	movl	52(%rsp), %edi
	cmpl	%ecx, %edi
	jge	.L161
	addl	$1, %edi
	movl	%edi, 8(%r8,%rdx,4)
.L161:
	movslq	48(%rsp), %rdi
	movq	%rsi, %rdx
	salq	$6, %rdx
	addq	%rsi, %rdx
	leaq	260(%rdi,%rdx,8), %rsi
	movq	32(%rsp), %rdi
	movl	8(%rdi,%rsi,4), %edx
	cmpl	%edx, %ecx
	jle	.L139
	addl	$1, %edx
	movl	%edx, 8(%rdi,%rsi,4)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L156:
	testl	%edx, %edx
	jle	.L139
	movq	32(%rsp), %rsi
	subl	$1, %edx
	movl	%edx, 8(%rsi,%rdi,4)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L131:
	movl	8(%r10), %edx
	cmpl	$2, %edx
	je	.L147
	ja	.L148
	testl	%edx, %edx
	.p2align 4,,2
	je	.L203
	xorl	%edx, %edx
	testl	%r9d, %r9d
	setne	%dl
	addl	%edx, %edx
	movl	%edx, 8(%r10)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L132:
	movl	8(%r10), %edx
	testl	%r9d, %r9d
	je	.L140
	cmpl	$1, %edx
	je	.L141
	cmpl	$2, %edx
	je	.L141
	testl	%edx, %edx
	.p2align 4,,3
	jne	.L139
	movl	$1, 8(%r10)
	jmp	.L139
.L171:
	movq	BTB@GOTPCREL(%rip), %rax
	movq	$-1, %r15
	movq	%rax, 32(%rsp)
	.p2align 4
	.p2align 3
.L126:
	movq	32(%rsp), %rbx
	movq	%r15, %rax
	salq	$6, %rax
	addq	%r15, %rax
	salq	$5, %rax
	addq	%rbx, %rax
	cmpl	$8, %r14d
	movl	44(%rsp), %ebx
	movq	%r12, (%rax)
	movl	%ebx, 2076(%rax)
	movq	24(%rsp), %rbx
	movq	%rbx, 16(%rax)
	ja	.L166
	cmpl	$5, %r14d
	ja	.L167
	movslq	40(%rsp), %rax
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	movq	RB@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rax,8), %rax
	xorl	%edx, %edx
	movslq	56(%rax), %rdi
	testl	%r9d, %r9d
	movslq	60(%rax), %rsi
	je	.L170
	movq	config@GOTPCREL(%rip), %rax
	movl	$1, %edx
	movl	112(%rax), %ecx
	sall	%cl, %edx
	subl	$1, %edx
.L170:
	movq	32(%rsp), %rbx
	movq	%r15, %rax
	salq	$6, %rax
	addq	%r15, %rax
	leaq	0(,%rax,8), %rcx
	salq	$5, %rax
	leaq	4(%rdi,%rcx), %rdi
	movl	%edx, 8(%rbx,%rdi,4)
	leaq	260(%rcx,%rsi), %rcx
	movl	$0, 2072(%rbx,%rax)
	movl	%edx, 8(%rbx,%rcx,4)
	leaq	72(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
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
.L200:
	.cfi_restore_state
	movq	fase_ejecucion_pred@GOTPCREL(%rip), %rax
	movl	(%rax), %r10d
	testl	%r10d, %r10d
	jne	.L123
	movq	f_trace@GOTPCREL(%rip), %rax
	movl	%ecx, 32(%rsp)
	movl	%edx, %ecx
	leaq	.LC6(%rip), %rdx
	movq	(%rax), %rdi
	xorl	%eax, %eax
	movq	%r8, (%rsp)
	movq	%rsi, %r8
	movl	$2, %esi
	call	__fprintf_chk@PLT
	movq	config@GOTPCREL(%rip), %rax
	movl	32(%rsp), %r9d
	movl	108(%rax), %r14d
	jmp	.L123
	.p2align 4,,7
	.p2align 3
.L140:
	cmpl	$2, %edx
	je	.L146
	cmpl	$3, %edx
	je	.L145
	cmpl	$1, %edx
	.p2align 4,,2
	jne	.L139
.L146:
	movl	$0, 8(%r10)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L157:
	cmpl	%edi, %r9d
	je	.L158
.L190:
	cmpl	%ecx, %r9d
	jne	.L158
	movq	config@GOTPCREL(%rip), %rdi
	movl	2072(%r10), %r8d
	movl	120(%rdi), %ecx
	movl	$1, %edi
	sall	%cl, %edi
	leal	-1(%rdi), %ecx
	cmpl	%ecx, %r8d
	jge	.L158
	addl	$1, %r8d
	movl	%r8d, 2072(%r10)
	jmp	.L158
	.p2align 4,,7
	.p2align 3
.L160:
	movq	32(%rsp), %rdi
	movq	%rsi, %rcx
	salq	$6, %rcx
	addq	%rsi, %rcx
	leaq	4(%rdx,%rcx,8), %rcx
	movl	8(%rdi,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L162
	subl	$1, %edx
	movl	%edx, 8(%rdi,%rcx,4)
.L162:
	movslq	48(%rsp), %rcx
	movq	%rsi, %rdx
	salq	$6, %rdx
	addq	%rsi, %rdx
	movq	32(%rsp), %rsi
	leaq	260(%rcx,%rdx,8), %rcx
	movl	8(%rsi,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L139
	subl	$1, %edx
	movl	%edx, 8(%rsi,%rcx,4)
	jmp	.L139
.L203:
	xorl	%edx, %edx
	testl	%r9d, %r9d
	setne	%dl
	movl	%edx, 8(%r10)
	jmp	.L139
.L166:
	cmpl	$10, %r14d
	jne	.L122
.L167:
	movq	32(%rsp), %rbx
	movq	%r15, %rax
	salq	$6, %rax
	addq	%r15, %rax
	salq	$5, %rax
	testl	%r9d, %r9d
	je	.L169
	movl	$3, 8(%rbx,%rax)
	jmp	.L122
.L141:
	movl	$3, 8(%r10)
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L202:
	movq	32(%rsp), %rax
	leaq	6(%rbx,%rdx), %rdx
	movl	%r9d, 48(%rsp)
	movl	%r9d, %edi
	movq	%r8, 56(%rsp)
	movl	%ecx, 52(%rsp)
	leaq	(%rax,%rdx,4), %rsi
	movl	%ecx, %edx
	call	cambia_estado_histeresis
	movq	56(%rsp), %r8
	movq	32(%rsp), %rax
	movl	48(%rsp), %edi
	movslq	60(%r8), %rdx
	leaq	262(%rbx,%rdx), %rdx
	leaq	(%rax,%rdx,4), %rsi
	movl	52(%rsp), %edx
	call	cambia_estado_histeresis
	movl	48(%rsp), %r9d
	leaq	.L130(%rip), %rax
	jmp	.L139
	.p2align 4,,7
	.p2align 3
.L148:
	cmpl	$3, %edx
	jne	.L139
	testl	%r9d, %r9d
	movl	$2, %ecx
	cmove	%ecx, %edx
	movl	%edx, 8(%r10)
	jmp	.L139
.L147:
	cmpl	$1, %r9d
	sbbl	%edx, %edx
	andl	$-2, %edx
	addl	$3, %edx
	movl	%edx, 8(%r10)
	jmp	.L139
.L169:
	movl	$0, 8(%rbx,%rax)
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L128:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$692, %r8d
	leaq	.LC2(%rip), %rcx
	leaq	.LC5(%rip), %rdx
	movl	$2, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L145:
	movl	$2, 8(%r10)
	jmp	.L139
	.cfi_endproc
.LFE57:
	.size	actualizar_prediccion, .-actualizar_prediccion
	.section	.rodata
	.align 16
	.type	__func__.0, @object
	.size	__func__.0, 19
__func__.0:
	.string	"obtener_prediccion"
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	-1
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

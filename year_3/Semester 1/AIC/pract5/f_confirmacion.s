	.file	"f_confirmacion.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"x"
.LC1:
	.string	"X"
	.text
	.p2align 4
	.globl	cancelar_instrucciones
	.type	cancelar_instrucciones, @function
cancelar_instrucciones:
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
	leaq	-8(%rsp), %rsp
	.cfi_def_cfa_offset 64
	movq	config@GOTPCREL(%rip), %r12
	movq	RB@GOTPCREL(%rip), %rbx
	movl	36(%r12), %eax
	testl	%eax, %eax
	jle	.L40
	movq	%rbx, %r14
	xorl	%r13d, %r13d
	movq	RB_inicio@GOTPCREL(%rip), %rbp
	leaq	.LC0(%rip), %r15
	.p2align 4
	.p2align 3
.L4:
	movl	(%r14), %eax
	testl	%eax, %eax
	je	.L3
	cmpl	%r13d, 0(%rbp)
	je	.L3
	movl	100(%r14), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	%r15, %rdi
	call	*72(%rax)
.L3:
	addl	$1, %r13d
	addq	$104, %r14
	cmpl	%r13d, 36(%r12)
	jg	.L4
.L2:
	movl	40(%r12), %eax
	testl	%eax, %eax
	jle	.L5
	movq	RS@GOTPCREL(%rip), %r13
	xorl	%r14d, %r14d
	leaq	.LC1(%rip), %r15
	jmp	.L8
	.p2align 4,,7
	.p2align 3
.L6:
	addl	$1, %r14d
	addq	$248, %r13
	cmpl	%r14d, 40(%r12)
	jle	.L5
.L8:
	movl	0(%r13), %eax
	testl	%eax, %eax
	je	.L6
	cmpl	$3, 204(%r13)
	jne	.L6
	movl	20(%r12), %eax
	addl	16(%r12), %eax
	addl	24(%r12), %eax
	cmpl	%r14d, %eax
	jg	.L7
	addl	28(%r12), %eax
	cmpl	%r14d, %eax
	jg	.L6
.L7:
	movl	244(%r13), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	%r15, %rdi
	call	*72(%rax)
	jmp	.L6
	.p2align 4,,7
	.p2align 3
.L5:
	movq	NUM_OPERADORES@GOTPCREL(%rip), %r13
	movslq	0(%r13), %rsi
	testl	%esi, %esi
	jle	.L9
	movq	Op@GOTPCREL(%rip), %r14
	xorl	%r15d, %r15d
	.p2align 4
	.p2align 3
.L12:
	movl	(%r14), %eax
	testl	%eax, %eax
	je	.L10
	movl	20(%r12), %eax
	movslq	8(%r14), %rdx
	addl	16(%r12), %eax
	addl	24(%r12), %eax
	cmpl	%eax, %edx
	jl	.L11
	addl	28(%r12), %eax
	cmpl	%eax, %edx
	jl	.L10
.L11:
	movq	%rdx, %rax
	leaq	.LC1(%rip), %rdi
	movq	RS@GOTPCREL(%rip), %r11
	salq	$5, %rax
	subq	%rdx, %rax
	xorl	%edx, %edx
	leaq	(%r11,%rax,8), %rax
	movl	244(%rax), %esi
	movq	presentacion@GOTPCREL(%rip), %rax
	call	*72(%rax)
.L10:
	movslq	0(%r13), %rsi
	addl	$1, %r15d
	addq	$28, %r14
	cmpl	%r15d, %esi
	jg	.L12
.L9:
	movq	Rfp@GOTPCREL(%rip), %rcx
	movl	$8, %eax
	movq	Rint@GOTPCREL(%rip), %rdx
	.p2align 4
	.p2align 3
.L13:
	movl	$4096, (%rax,%rcx)
	addq	$16, %rax
	movl	$4096, -16(%rax,%rdx)
	cmpq	$520, %rax
	jne	.L13
	movslq	40(%r12), %rdx
	testl	%edx, %edx
	jle	.L14
	movq	RS@GOTPCREL(%rip), %rax
	movq	%rdx, %rcx
	salq	$5, %rcx
	subq	%rdx, %rcx
	leaq	(%rax,%rcx,8), %rdx
	.p2align 4
	.p2align 3
.L17:
	movl	(%rax), %r10d
	testl	%r10d, %r10d
	je	.L15
	movl	200(%rax), %r9d
	testl	%r9d, %r9d
	jne	.L16
.L15:
	movl	$0, (%rax)
.L16:
	addq	$248, %rax
	cmpq	%rdx, %rax
	jne	.L17
.L14:
	testl	%esi, %esi
	jle	.L18
	movq	Op@GOTPCREL(%rip), %rax
	leaq	0(,%rsi,8), %rdx
	subq	%rsi, %rdx
	leaq	(%rax,%rdx,4), %rcx
	jmp	.L21
	.p2align 4,,7
	.p2align 3
.L20:
	movl	$0, (%rax)
.L19:
	addq	$28, %rax
	cmpq	%rcx, %rax
	je	.L18
.L21:
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	je	.L19
	cmpl	$3, 4(%rax)
	jne	.L20
	movslq	8(%rax), %rsi
	movq	%rsi, %rdx
	salq	$5, %rdx
	subq	%rsi, %rdx
	movq	RS@GOTPCREL(%rip), %rsi
	leaq	(%rsi,%rdx,8), %rdx
	movl	200(%rdx), %edi
	testl	%edi, %edi
	je	.L20
	addq	$28, %rax
	cmpq	%rcx, %rax
	jne	.L21
	.p2align 4
	.p2align 3
.L18:
	movslq	0(%rbp), %rax
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	leaq	(%rbx,%rax,8), %rax
	movl	40(%rax), %ecx
	testl	%ecx, %ecx
	jne	.L22
	movq	88(%rax), %rax
	addq	$4, %rax
.L23:
	movq	Control_1@GOTPCREL(%rip), %rdx
	movq	%rax, (%rdx)
	movq	Cancelar_Activa@GOTPCREL(%rip), %rax
	movslq	36(%r12), %rdx
	movl	$1, (%rax)
	movq	Excepcion_Activa@GOTPCREL(%rip), %rax
	testl	%edx, %edx
	movl	$0, (%rax)
	jle	.L24
	leaq	(%rdx,%rdx,2), %rcx
	movq	%rbx, %rax
	leaq	(%rdx,%rcx,4), %rdx
	leaq	(%rbx,%rdx,8), %rdx
	.p2align 4
	.p2align 3
.L25:
	movl	$0, (%rax)
	addq	$104, %rax
	movl	$0, -24(%rax)
	cmpq	%rdx, %rax
	jne	.L25
.L24:
	movq	L1D@GOTPCREL(%rip), %rdi
	movq	RB_long@GOTPCREL(%rip), %rax
	movl	$0, 0(%rbp)
	movl	(%rdi), %edx
	movl	$0, (%rax)
	movq	RB_fin@GOTPCREL(%rip), %rax
	testl	%edx, %edx
	movl	$0, (%rax)
	jne	.L41
	movq	L1I@GOTPCREL(%rip), %rdi
	movl	(%rdi), %eax
	testl	%eax, %eax
	jne	.L42
.L1:
	leaq	8(%rsp), %rsp
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
.L22:
	.cfi_restore_state
	movq	32(%rax), %rax
	jmp	.L23
.L41:
	movl	$1, %esi
	call	cancela_operaciones_mem_cache@PLT
	movq	L1I@GOTPCREL(%rip), %rdi
	movl	(%rdi), %eax
	testl	%eax, %eax
	je	.L1
.L42:
	leaq	8(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	$1, %esi
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
	jmp	cancela_operaciones_mem_cache@PLT
.L40:
	.cfi_restore_state
	movq	RB_inicio@GOTPCREL(%rip), %rbp
	jmp	.L2
	.cfi_endproc
.LFE39:
	.size	cancelar_instrucciones, .-cancelar_instrucciones
	.p2align 4
	.globl	prediccion_correcta
	.type	prediccion_correcta, @function
prediccion_correcta:
.LFB40:
	.cfi_startproc
	endbr64
	movq	RB_inicio@GOTPCREL(%rip), %rax
	movslq	(%rax), %rax
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	movq	RB@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rax,8), %rax
	movslq	4(%rax), %rdx
	movl	44(%rax), %esi
	movl	40(%rax), %ecx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$4, %rdx
	addq	instruction_list@GOTPCREL(%rip), %rdx
	movzbl	44(%rdx), %edx
	andl	$119, %edx
	cmpb	$103, %dl
	je	.L44
	xorl	%edx, %edx
	cmpl	%ecx, %esi
	sete	%dl
.L43:
	movl	%edx, %eax
	ret
	.p2align 4,,7
	.p2align 3
.L44:
	xorl	%edx, %edx
	cmpl	%ecx, %esi
	jne	.L43
	movq	72(%rax), %rdi
	xorl	%edx, %edx
	cmpq	%rdi, 32(%rax)
	sete	%dl
	movl	%edx, %eax
	ret
	.cfi_endproc
.LFE40:
	.size	prediccion_correcta, .-prediccion_correcta
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Ciclo %u: Acceso a memoria de instrucciones ilegal. PC=%ld (%s)\n"
	.align 8
.LC3:
	.string	"Ciclo %u: Acceso a memoria de datos ilegal. PC=%ld (%s)\n"
	.align 8
.LC4:
	.string	"Ciclo %u: Acceso desalineado a memoria de datos. PC=%ld (%s)\n"
	.align 8
.LC5:
	.string	"Ciclo %u: Excepci\303\263n no manejada. PC=%ld (%s)\n"
	.section	.rodata.str1.1
.LC6:
	.string	"C"
.LC7:
	.string	"f_confirmacion.c"
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"ERROR (%s:%d): Operacion %d no implementada\n"
	.text
	.p2align 4
	.globl	fase_COM
	.type	fase_COM, @function
fase_COM:
.LFB43:
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
	leaq	-152(%rsp), %rsp
	.cfi_def_cfa_offset 208
	movq	terminando@GOTPCREL(%rip), %r12
	movq	RB_inicio@GOTPCREL(%rip), %rbx
	movq	RB_cab@GOTPCREL(%rip), %rdx
	movl	(%r12), %esi
	movq	%fs:40, %rax
	movq	%rax, 136(%rsp)
	xorl	%eax, %eax
	movslq	(%rbx), %rax
	testl	%esi, %esi
	movl	%eax, (%rdx)
	jne	.L48
	movq	RB@GOTPCREL(%rip), %rbp
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rdx
	movl	(%rdx), %ecx
	testl	%ecx, %ecx
	jne	.L174
.L48:
	movq	136(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L167
	leaq	152(%rsp), %rsp
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
.L174:
	.cfi_restore_state
	movl	8(%rdx), %r15d
	movl	4(%rdx), %ecx
	movq	instruction_list@GOTPCREL(%rip), %r13
	testl	%r15d, %r15d
	movslq	%ecx, %rdx
	je	.L146
.L50:
	leaq	(%rax,%rax,2), %rsi
	leaq	(%rax,%rsi,4), %rsi
	movl	80(%rbp,%rsi,8), %r11d
	testl	%r11d, %r11d
	jne	.L55
	leaq	(%rdx,%rdx,2), %rsi
	salq	$4, %rsi
	movl	40(%r13,%rsi), %esi
	salq	$4, %rsi
	addq	inst_types_list@GOTPCREL(%rip), %rsi
	movl	(%rsi), %esi
	subl	$6, %esi
	andl	$-3, %esi
	je	.L175
.L55:
	movq	estat@GOTPCREL(%rip), %r14
	leaq	(%rdx,%rdx,2), %rdx
	salq	$4, %rdx
	addq	$1, 8(%r14)
	movzbl	44(%r13,%rdx), %edx
	andl	$123, %edx
	cmpb	$35, %dl
	je	.L57
	cmpl	$136, %ecx
	jg	.L58
	cmpl	$102, %ecx
	jle	.L60
	subl	$103, %ecx
.L157:
	movabsq	$8594130945, %rdx
	btq	%rcx, %rdx
	jnc	.L60
.L57:
	movq	RS@GOTPCREL(%rip), %r10
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rcx
	movq	16(%rcx), %rsi
	movq	%rsi, %rdx
	salq	$5, %rdx
	subq	%rsi, %rdx
	leaq	(%r10,%rdx,8), %rdx
	movl	232(%rdx), %edx
	movl	%edx, 80(%rcx)
.L60:
	leaq	(%rax,%rax,2), %rdx
	movq	%rsp, %r15
	leaq	(%rax,%rdx,4), %rax
	movq	%r15, %rdi
	movq	88(%rbp,%rax,8), %rsi
	xorl	%edx, %edx
	call	imprime_instruccion_color@PLT
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	movslq	(%rbx), %rax
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rcx
	movl	80(%rcx), %edx
	cmpl	$2, %edx
	je	.L62
	jg	.L63
	testl	%edx, %edx
	je	.L64
	cmpl	$1, %edx
	.p2align 4,,6
	jne	.L66
	movq	Ciclo@GOTPCREL(%rip), %rax
	movq	%r15, %r9
	movq	stderr@GOTPCREL(%rip), %rdx
	movl	$2, %esi
	movq	88(%rcx), %r8
	movl	(%rax), %eax
	movq	(%rdx), %rdi
	leaq	.LC2(%rip), %rdx
	movl	%eax, %ecx
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movslq	(%rbx), %rax
	movl	$1, (%r12)
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	movl	80(%rbp,%rdx,8), %edx
	jmp	.L69
	.p2align 4,,7
	.p2align 3
.L146:
	leaq	(%rdx,%rdx,2), %rsi
	salq	$4, %rsi
	movzbl	44(%r13,%rsi), %esi
	andl	$123, %esi
	cmpb	$35, %sil
	je	.L51
	cmpl	$136, %ecx
	jg	.L52
	cmpl	$102, %ecx
	leal	-103(%rcx), %esi
	jle	.L48
.L166:
	movabsq	$8594130945, %rdi
	btq	%rsi, %rdi
	jnc	.L48
.L51:
	movq	RS@GOTPCREL(%rip), %r14
	leaq	(%rax,%rax,2), %rsi
	leaq	(%rax,%rsi,4), %rsi
	movq	16(%rbp,%rsi,8), %rdi
	movq	%rdi, %rsi
	salq	$5, %rsi
	subq	%rdi, %rsi
	leaq	(%r14,%rsi,8), %rsi
	cmpl	$1, 204(%rsi)
	jne	.L48
	jmp	.L50
	.p2align 4,,7
	.p2align 3
.L63:
	cmpl	$4, %edx
	je	.L67
	cmpl	$255, %edx
	.p2align 4,,2
	jne	.L66
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rdi
	movq	presentacion@GOTPCREL(%rip), %rcx
	movl	$1, (%rdi)
	jmp	.L80
	.p2align 4,,7
	.p2align 3
.L58:
	subl	$167, %ecx
	cmpl	$33, %ecx
	jbe	.L157
	jmp	.L60
	.p2align 4,,7
	.p2align 3
.L67:
	movq	Ciclo@GOTPCREL(%rip), %rax
	movq	%r15, %r9
	movq	stderr@GOTPCREL(%rip), %rdx
	movq	88(%rcx), %r8
	movl	(%rax), %eax
	movq	(%rdx), %rdi
	leaq	.LC4(%rip), %rdx
	movl	%eax, %ecx
.L173:
	xorl	%eax, %eax
	movl	$2, %esi
	call	__fprintf_chk@PLT
	movslq	(%rbx), %rax
	movl	$1, (%r12)
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	movslq	4(%rbp,%rdx,8), %rdx
	movq	%rdx, %rcx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$4, %rdx
	movzbl	44(%r13,%rdx), %edx
	andl	$123, %edx
	cmpb	$35, %dl
	je	.L75
	cmpl	$136, %ecx
	jg	.L76
	cmpl	$102, %ecx
	jle	.L78
	subl	$103, %ecx
.L161:
	movabsq	$8594130945, %rdx
	btq	%rcx, %rdx
	jnc	.L78
.L75:
	movq	RS@GOTPCREL(%rip), %r9
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rcx
	movq	16(%rcx), %rsi
	movq	%rsi, %rdx
	salq	$5, %rdx
	subq	%rsi, %rdx
	leaq	(%r9,%rdx,8), %rdx
	movl	$0, (%rdx)
	movl	80(%rcx), %edx
.L69:
	movq	presentacion@GOTPCREL(%rip), %rcx
	leaq	(%rax,%rax,2), %rsi
	testl	%edx, %edx
	leaq	(%rax,%rsi,4), %rsi
	movl	100(%rbp,%rsi,8), %esi
	movq	72(%rcx), %r8
	je	.L81
.L80:
	leaq	(%rax,%rax,2), %rdx
	leaq	.LC6(%rip), %rdi
	leaq	(%rax,%rdx,4), %rax
	movl	$1, %edx
	leaq	0(%rbp,%rax,8), %rax
	movl	100(%rax), %esi
	movl	$40, 4(%rax)
	call	*72(%rcx)
.L82:
	movslq	(%rbx), %rax
	leaq	(%rax,%rax,2), %rdx
	movq	%rax, %rdi
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rsi
	movl	4(%rsi), %r9d
	cmpl	$40, %r9d
	je	.L176
	movslq	%r9d, %rcx
	leaq	(%rcx,%rcx,2), %rdx
	salq	$4, %rdx
	movzbl	44(%r13,%rdx), %r8d
	movl	%r8d, %edx
	andl	$127, %edx
	cmpb	$39, %dl
	je	.L89
	jbe	.L177
	addl	$29, %edx
	andl	$127, %edx
	cmpb	$12, %dl
	ja	.L91
	movl	$4113, %r10d
	btq	%rdx, %r10
	jnc	.L91
	andl	$119, %r8d
	cmpb	$103, %r8b
	jne	.L95
	movq	16(%rsi), %rdx
	movq	Rint@GOTPCREL(%rip), %rcx
	testq	%rdx, %rdx
	je	.L96
	movq	24(%rsi), %rsi
	movq	%rdx, %r8
	salq	$4, %r8
	movq	%rsi, (%rcx,%r8)
.L96:
	salq	$4, %rdx
	addq	%rcx, %rdx
	cmpl	8(%rdx), %edi
	jne	.L95
	movl	$4096, 8(%rdx)
	.p2align 4
	.p2align 3
.L95:
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	leaq	0(%rbp,%rax,8), %rax
	movl	80(%rax), %r8d
	testl	%r8d, %r8d
	je	.L178
.L98:
	addq	$1, 56(%r14)
	call	prediccion_correcta@PLT
	testl	%eax, %eax
	je	.L99
	movslq	(%rbx), %rax
	addq	$1, 64(%r14)
	movq	config@GOTPCREL(%rip), %r13
	movq	%rax, %rdi
.L88:
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	leaq	0(%rbp,%rax,8), %rax
	movl	$0, (%rax)
	movl	$0, 80(%rax)
	leal	1(%rdi), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	36(%r13)
	movq	RB_long@GOTPCREL(%rip), %rax
	subl	$1, (%rax)
	movl	%edx, (%rbx)
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L64:
	movq	presentacion@GOTPCREL(%rip), %rax
	movl	100(%rcx), %esi
	movq	72(%rax), %r8
.L81:
	xorl	%edx, %edx
	leaq	.LC6(%rip), %rdi
	call	*%r8
	jmp	.L82
	.p2align 4,,7
	.p2align 3
.L52:
	leal	-167(%rcx), %esi
	cmpl	$33, %esi
	jbe	.L166
	.p2align 4,,2
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L176:
	movq	config@GOTPCREL(%rip), %r13
	movl	20(%r13), %edx
	movl	28(%r13), %esi
	addl	16(%r13), %edx
	addl	24(%r13), %edx
	leal	(%rsi,%rdx), %ecx
	cmpl	%edx, %ecx
	jle	.L84
	movq	RS@GOTPCREL(%rip), %r8
	movslq	%edx, %rcx
	movq	%rcx, %rdx
	addq	%rcx, %rsi
	salq	$5, %rdx
	subq	%rcx, %rdx
	movq	%rsi, %rcx
	salq	$5, %rcx
	leaq	(%r8,%rdx,8), %rdx
	subq	%rsi, %rcx
	leaq	(%r8,%rcx,8), %rsi
	jmp	.L86
	.p2align 4,,7
	.p2align 3
.L179:
	addq	$248, %rdx
	cmpq	%rsi, %rdx
	je	.L84
.L86:
	movl	(%rdx), %ecx
	andl	200(%rdx), %ecx
	je	.L179
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L177:
	cmpb	$35, %dl
	jne	.L91
.L89:
	movq	config@GOTPCREL(%rip), %r13
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	movq	16(%rbp,%rdx,8), %rcx
	movq	%rcx, %rdx
	salq	$5, %rdx
	subq	%rcx, %rdx
	movq	RS@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rdx,8), %rdx
	movl	$1, 200(%rdx)
	jmp	.L88
	.p2align 4,,7
	.p2align 3
.L91:
	cmpl	$136, %r9d
	jg	.L101
	cmpl	$102, %r9d
	leal	-103(%r9), %edx
	jle	.L103
.L163:
	movabsq	$8594130945, %rsi
	btq	%rdx, %rsi
	jc	.L89
.L103:
	leaq	(%rcx,%rcx,2), %rdx
	salq	$4, %rdx
	movl	40(%r13,%rdx), %edx
	salq	$4, %rdx
	addq	inst_types_list@GOTPCREL(%rip), %rdx
	movl	(%rdx), %esi
	cmpl	$5, %esi
	je	.L180
	leal	-6(%rsi), %edx
	andl	$-3, %edx
	jne	.L108
	leaq	(%rax,%rax,2), %rdx
	cmpl	$8, %esi
	leaq	(%rax,%rdx,4), %rax
	sete	%sil
	movq	16(%rbp,%rax,8), %rdi
	movzbl	%sil, %esi
	movl	$1, %edx
	leal	6(%rsi,%rsi), %esi
	call	marca_fp_reg@PLT
	movslq	(%rbx), %rax
	leaq	(%rax,%rax,2), %rdx
	movq	%rax, %rdi
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rcx
	movq	16(%rcx), %rdx
	movq	24(%rcx), %rcx
	salq	$4, %rdx
	addq	Rfp@GOTPCREL(%rip), %rdx
	movq	%rcx, (%rdx)
	cmpl	8(%rdx), %eax
	jne	.L110
	movl	$4096, 8(%rdx)
.L110:
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	movslq	4(%rbp,%rdx,8), %rcx
	movq	%rcx, %rdx
	leaq	(%rcx,%rcx,2), %rcx
	salq	$4, %rcx
	movzbl	44(%r13,%rcx), %ecx
	andl	$127, %ecx
	cmpb	$7, %cl
	je	.L164
	movq	32(%r14), %rcx
	leal	-52(%rdx), %esi
	cmpl	$3, %esi
	jbe	.L112
	subl	$78, %edx
	cmpl	$3, %edx
	jbe	.L112
	movq	config@GOTPCREL(%rip), %r13
	addq	$1, %rcx
	movq	%rcx, 32(%r14)
	jmp	.L88
	.p2align 4,,7
	.p2align 3
.L62:
	movq	Ciclo@GOTPCREL(%rip), %rax
	movq	%r15, %r9
	movq	stderr@GOTPCREL(%rip), %rdx
	movq	88(%rcx), %r8
	movl	(%rax), %eax
	movq	(%rdx), %rdi
	leaq	.LC3(%rip), %rdx
	movl	%eax, %ecx
	jmp	.L173
	.p2align 4,,7
	.p2align 3
.L175:
	movq	136(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L167
	leaq	152(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
	jmp	fase_COM_alum@PLT
	.p2align 4,,7
	.p2align 3
.L84:
	.cfi_restore_state
	leaq	(%rax,%rax,2), %rdx
	movl	$1, %ecx
	leaq	(%rax,%rdx,4), %rax
	leaq	0(%rbp,%rax,8), %rax
	movq	88(%rax), %rsi
	movl	100(%rax), %edx
	leaq	4(%rsi), %r8
	call	actualizar_prediccion@PLT
	call	cancelar_instrucciones@PLT
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rax
	cmpl	$1, (%rax)
	je	.L87
	movslq	(%rbx), %rax
	movq	%rax, %rdi
	jmp	.L88
	.p2align 4,,7
	.p2align 3
.L76:
	subl	$167, %ecx
	cmpl	$33, %ecx
	jbe	.L161
	.p2align 4
	.p2align 3
.L78:
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rdx
	movl	80(%rbp,%rdx,8), %edx
	jmp	.L69
	.p2align 4,,7
	.p2align 3
.L101:
	leal	-167(%r9), %edx
	cmpl	$33, %edx
	jbe	.L163
	jmp	.L103
	.p2align 4,,7
	.p2align 3
.L99:
	movslq	(%rbx), %rax
	movl	$1, %edi
	movq	config@GOTPCREL(%rip), %r10
	leaq	(%rax,%rax,2), %rdx
	movslq	128(%r10), %rcx
	leaq	(%rax,%rdx,4), %rax
	leaq	0(%rbp,%rax,8), %r9
	movq	88(%r9), %r8
	movl	40(%r9), %r11d
	movq	%r8, %rdx
	movq	%r8, %rax
	sarq	$63, %rdx
	idivq	%rcx
	movl	56(%r9), %eax
	movl	132(%r10), %ecx
	leal	(%rax,%rax), %esi
	movl	%edi, %eax
	sall	%cl, %eax
	orl	%r11d, %esi
	movslq	136(%r10), %rcx
	subl	$1, %eax
	andl	%eax, %esi
	movq	BHSR1@GOTPCREL(%rip), %rax
	movl	%esi, (%rax,%rdx,4)
	movq	%r8, %rdx
	sarq	$63, %rdx
	movq	%r8, %rax
	idivq	%rcx
	movl	60(%r9), %eax
	movl	140(%r10), %ecx
	sall	%cl, %edi
	addl	%eax, %eax
	orl	%r11d, %eax
	leal	-1(%rdi), %ecx
	andl	%ecx, %eax
	movq	BHSR2@GOTPCREL(%rip), %rcx
	movl	%eax, (%rcx,%rdx,4)
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	(%rax), %esi
	testl	%esi, %esi
	je	.L100
	movq	orden_clarividente@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edi
	cmpl	%edi, 100(%r9)
	jne	.L100
	movl	$0, (%rax)
	.p2align 4
	.p2align 3
.L100:
	call	cancelar_instrucciones@PLT
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L178:
	movl	40(%rax), %ecx
	movl	100(%rax), %edx
	movq	88(%rax), %rsi
	movq	32(%rax), %r8
	call	actualizar_prediccion@PLT
	jmp	.L98
.L108:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$365, %r8d
	leaq	.LC7(%rip), %rcx
	leaq	.LC8(%rip), %rdx
	movl	$2, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movslq	(%rbx), %rax
	movq	%rax, %rdi
.L164:
	movq	config@GOTPCREL(%rip), %r13
	jmp	.L88
.L112:
	movq	config@GOTPCREL(%rip), %r13
	addq	$2, %rcx
	movq	%rcx, 32(%r14)
	jmp	.L88
.L180:
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	movl	$1, %edx
	movq	16(%rbp,%rax,8), %rdi
	call	marca_int_reg@PLT
	movslq	(%rbx), %rax
	movq	Rint@GOTPCREL(%rip), %rcx
	leaq	(%rax,%rax,2), %rdx
	movq	%rax, %rdi
	leaq	(%rax,%rdx,4), %rdx
	leaq	0(%rbp,%rdx,8), %rsi
	movq	16(%rsi), %rdx
	testq	%rdx, %rdx
	je	.L106
	movq	24(%rsi), %rsi
	movq	%rdx, %r8
	salq	$4, %r8
	movq	%rsi, (%rcx,%r8)
.L106:
	salq	$4, %rdx
	addq	%rdx, %rcx
	cmpl	8(%rcx), %edi
	jne	.L164
	movl	$4096, 8(%rcx)
	movq	config@GOTPCREL(%rip), %r13
	jmp	.L88
.L87:
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	call	process_syscall@PLT
	movl	%eax, (%r12)
	jmp	.L48
.L167:
	call	__stack_chk_fail@PLT
.L66:
	imulq	$104, %rax, %rax
	movq	%r15, %r9
	leaq	.LC5(%rip), %rdx
	movl	$2, %esi
	movq	88(%rbp,%rax), %r8
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	movq	stderr@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE43:
	.size	fase_COM, .-fase_COM
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

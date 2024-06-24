	.text
	.file	"expressions2"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	x@GOTPCREL(%rip), %r14
	movq	%r14, %rdi
	callq	readln@PLT
	movq	y@GOTPCREL(%rip), %rbx
	movq	%rbx, %rdi
	callq	readln@PLT
	movl	(%rbx), %eax
	movl	(%r14), %edi
	leal	(%rdi,%rax), %ecx
	movq	a@GOTPCREL(%rip), %r15
	movl	%ecx, (%r15)
	subl	%edi, %eax
	movq	b@GOTPCREL(%rip), %r12
	movl	%eax, (%r12)
                                        # kill: def $edi killed $edi killed $rdi
	callq	writeln@PLT
	movl	(%rbx), %edi
	callq	writeln@PLT
	movl	(%r15), %edi
	callq	writeln@PLT
	movl	(%r12), %edi
	callq	writeln@PLT
	movl	(%r14), %eax
	addl	(%r15), %eax
	movl	(%rbx), %edi
	subl	(%r12), %edi
	imull	%eax, %edi
	movq	c@GOTPCREL(%rip), %rax
	movl	%edi, (%rax)
	callq	writeln@PLT
	movl	(%r15), %eax
	xorl	%edx, %edx
	divl	(%r12)
	movl	%edx, %edi
	movq	d@GOTPCREL(%rip), %rax
	movl	%edx, (%rax)
	callq	writeln@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	a,@object                       # @a
	.comm	a,4,4
	.type	b,@object                       # @b
	.comm	b,4,4
	.type	c,@object                       # @c
	.comm	c,4,4
	.type	d,@object                       # @d
	.comm	d,4,4
	.type	x,@object                       # @x
	.comm	x,4,4
	.type	y,@object                       # @y
	.comm	y,4,4
	.section	".note.GNU-stack","",@progbits

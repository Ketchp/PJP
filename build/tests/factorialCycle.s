	.text
	.file	"factorialCycle"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	f@GOTPCREL(%rip), %r14
	movl	$1, (%r14)
	movq	n@GOTPCREL(%rip), %rbx
	movq	%rbx, %rdi
	callq	readln@PLT
	cmpl	$2, (%rbx)
	jl	.LBB0_3
	.p2align	4, 0x90
.LBB0_2:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %eax
	movl	(%r14), %ecx
	imull	%eax, %ecx
	movl	%ecx, (%r14)
	decl	%eax
	movl	%eax, (%rbx)
	cmpl	$2, (%rbx)
	jge	.LBB0_2
.LBB0_3:                                # %after
	movl	(%r14), %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	f,@object                       # @f
	.comm	f,4,4
	.type	n,@object                       # @n
	.comm	n,4,4
	.section	".note.GNU-stack","",@progbits

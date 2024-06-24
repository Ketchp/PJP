	.text
	.file	"fibonacci"
	.globl	fibonacci                       # -- Begin function fibonacci
	.p2align	4, 0x90
	.type	fibonacci,@function
fibonacci:                              # @fibonacci
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 8(%rsp)
	cmpl	$1, %edi
	jg	.LBB0_2
# %bb.1:                                # %body
	movl	8(%rsp), %eax
	jmp	.LBB0_3
.LBB0_2:                                # %else
	movl	8(%rsp), %edi
	decl	%edi
	callq	fibonacci@PLT
	movl	%eax, %ebx
	movl	8(%rsp), %edi
	addl	$-2, %edi
	callq	fibonacci@PLT
	addl	%ebx, %eax
.LBB0_3:                                # %after
	movl	%eax, 12(%rsp)
	movl	12(%rsp), %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	fibonacci, .Lfunc_end0-fibonacci
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$8, %edi
	callq	fibonacci@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$9, %edi
	callq	fibonacci@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits

	.text
	.file	"factorialRec"
	.globl	fact                            # -- Begin function fact
	.p2align	4, 0x90
	.type	fact,@function
fact:                                   # @fact
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	testl	%edi, %edi
	jne	.LBB0_2
# %bb.1:                                # %body
	movl	$1, 8(%rsp)
	jmp	.LBB0_3
.LBB0_2:                                # %else
	movl	12(%rsp), %ebx
	leal	-1(%rbx), %edi
	callq	fact@PLT
	imull	%ebx, %eax
	movl	%eax, 8(%rsp)
.LBB0_3:                                # %after
	movl	8(%rsp), %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	fact, .Lfunc_end0-fact
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	n@GOTPCREL(%rip), %rbx
	movq	%rbx, %rdi
	callq	readln@PLT
	movl	(%rbx), %edi
	callq	fact@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	n,@object                       # @n
	.comm	n,4,4
	.section	".note.GNU-stack","",@progbits

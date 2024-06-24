	.text
	.file	"indirectrecursion"
	.globl	iseven                          # -- Begin function iseven
	.p2align	4, 0x90
	.type	iseven,@function
iseven:                                 # @iseven
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, (%rsp)
	testl	%edi, %edi
	jle	.LBB0_2
# %bb.1:                                # %body
	movl	(%rsp), %edi
	decl	%edi
	callq	isodd@PLT
	movl	%eax, 4(%rsp)
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.LBB0_2:                                # %else
	.cfi_def_cfa_offset 16
	movl	$1, 4(%rsp)
	movl	$1, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	iseven, .Lfunc_end0-iseven
	.cfi_endproc
                                        # -- End function
	.globl	isodd                           # -- Begin function isodd
	.p2align	4, 0x90
	.type	isodd,@function
isodd:                                  # @isodd
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, (%rsp)
	testl	%edi, %edi
	jle	.LBB1_2
# %bb.1:                                # %body
	movl	(%rsp), %edi
	decl	%edi
	callq	iseven@PLT
	movl	%eax, 4(%rsp)
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.LBB1_2:                                # %else
	.cfi_def_cfa_offset 16
	movl	$0, 4(%rsp)
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	isodd, .Lfunc_end1-isodd
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
	movl	$11, %edi
	callq	iseven@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$11, %edi
	callq	isodd@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits

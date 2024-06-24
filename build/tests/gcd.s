	.text
	.file	"gcd"
	.globl	gcdi                            # -- Begin function gcdi
	.p2align	4, 0x90
	.type	gcdi,@function
gcdi:                                   # @gcdi
	.cfi_startproc
# %bb.0:                                # %entry
	movl	%edi, -12(%rsp)
	movl	%esi, -16(%rsp)
	cmpl	$0, -16(%rsp)
	je	.LBB0_3
	.p2align	4, 0x90
.LBB0_2:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movl	-16(%rsp), %ecx
	movl	%ecx, -8(%rsp)
	movl	-12(%rsp), %eax
	xorl	%edx, %edx
	divl	%ecx
	movl	%edx, -16(%rsp)
	movl	%ecx, -12(%rsp)
	cmpl	$0, -16(%rsp)
	jne	.LBB0_2
.LBB0_3:                                # %after
	movl	-12(%rsp), %eax
	movl	%eax, -4(%rsp)
	retq
.Lfunc_end0:
	.size	gcdi, .Lfunc_end0-gcdi
	.cfi_endproc
                                        # -- End function
	.globl	gcdr                            # -- Begin function gcdr
	.p2align	4, 0x90
	.type	gcdr,@function
gcdr:                                   # @gcdr
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, %eax
	movl	%edi, 20(%rsp)
	movl	%esi, 8(%rsp)
	xorl	%edx, %edx
	divl	%esi
	movl	%edx, 12(%rsp)
	testl	%edx, %edx
	jne	.LBB1_3
# %bb.1:                                # %body
	movl	8(%rsp), %eax
	jmp	.LBB1_2
.LBB1_3:                                # %else
	movl	8(%rsp), %edi
	movl	12(%rsp), %esi
	callq	gcdr@PLT
.LBB1_2:                                # %body
	movl	%eax, 16(%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	gcdr, .Lfunc_end1-gcdr
	.cfi_endproc
                                        # -- End function
	.globl	gcdr_guessing                   # -- Begin function gcdr_guessing
	.p2align	4, 0x90
	.type	gcdr_guessing,@function
gcdr_guessing:                          # @gcdr_guessing
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, 16(%rsp)
	movl	%esi, 12(%rsp)
	movl	%esi, %edx
	callq	gcdr_guessing_inner@PLT
	movl	%eax, 20(%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	gcdr_guessing, .Lfunc_end2-gcdr_guessing
	.cfi_endproc
                                        # -- End function
	.globl	gcdr_guessing_inner             # -- Begin function gcdr_guessing_inner
	.p2align	4, 0x90
	.type	gcdr_guessing_inner,@function
gcdr_guessing_inner:                    # @gcdr_guessing_inner
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edx, %ecx
	movl	%edi, %eax
	movl	%edi, 16(%rsp)
	movl	%esi, 12(%rsp)
	movl	%edx, 8(%rsp)
	xorl	%edx, %edx
	divl	%ecx
	movl	%edx, %edi
	movl	%esi, %eax
	xorl	%edx, %edx
	divl	%ecx
	orl	%edi, %edx
	jne	.LBB3_3
# %bb.1:                                # %body
	movl	8(%rsp), %eax
	jmp	.LBB3_2
.LBB3_3:                                # %else
	movl	16(%rsp), %edi
	movl	12(%rsp), %esi
	movl	8(%rsp), %edx
	decl	%edx
	callq	gcdr_guessing_inner@PLT
.LBB3_2:                                # %body
	movl	%eax, 20(%rsp)
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	gcdr_guessing_inner, .Lfunc_end3-gcdr_guessing_inner
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
	movl	$54, %edi
	movl	$81, %esi
	callq	gcdi@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$54, %edi
	movl	$81, %esi
	callq	gcdr@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$54, %edi
	movl	$81, %esi
	callq	gcdr_guessing@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$5, %edi
	movl	$7, %esi
	callq	gcdi@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$5, %edi
	movl	$7, %esi
	callq	gcdr@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$5, %edi
	movl	$7, %esi
	callq	gcdr_guessing@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$4, %edi
	movl	$12, %esi
	callq	gcdi@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$4, %edi
	movl	$12, %esi
	callq	gcdr@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$4, %edi
	movl	$12, %esi
	callq	gcdr_guessing@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$8, %edi
	movl	$12, %esi
	callq	gcdi@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$8, %edi
	movl	$12, %esi
	callq	gcdr@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	movl	$8, %edi
	movl	$12, %esi
	callq	gcdr_guessing@PLT
	movl	%eax, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	main, .Lfunc_end4-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits

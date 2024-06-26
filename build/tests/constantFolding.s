	.text
	.file	"consts"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$3, %edi
	callq	writeln@PLT
	movl	$26, %edi
	callq	writeln@PLT
	movl	$299, %edi                      # imm = 0x12B
	callq	writeln@PLT
	movl	$325, %edi                      # imm = 0x145
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits

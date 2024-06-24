	.text
	.file	"inputOutput"
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
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	n,@object                       # @n
	.comm	n,4,4
	.section	".note.GNU-stack","",@progbits

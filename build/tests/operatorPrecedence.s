	.text
	.file	"sortBubble"
	.globl	f                               # -- Begin function f
	.p2align	4, 0x90
	.type	f,@function
f:                                      # @f
	.cfi_startproc
# %bb.0:                                # %entry
	movl	%edi, %eax
	movl	%edi, -8(%rsp)
	movl	%edi, -4(%rsp)
	retq
.Lfunc_end0:
	.size	f, .Lfunc_end0-f
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movq	ARR@GOTPCREL(%rip), %r14
	movabsq	$858993459300, %rax             # imm = 0xC800000064
	movq	%rax, 4(%r14)
	movl	$300, 12(%r14)                  # imm = 0x12C
	movl	$1, %edi
	callq	f@PLT
	movl	$-299, %edi                     # imm = 0xFED5
	callq	writeln@PLT
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	subl	4(%r14), %ebp
	movl	$1, %edi
	callq	f@PLT
	movl	%ebp, %edi
	callq	writeln@PLT
	subl	8(%r14), %ebx
	movl	$1, %edi
	callq	f@PLT
	movl	%ebx, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	ARR,@object                     # @ARR
	.comm	ARR,44,16
	.type	.Lstr_literal,@object           # @str_literal
	.section	.rodata,"a",@progbits
.Lstr_literal:
	.asciz	"No parentheses!"
	.size	.Lstr_literal, 16

	.type	.Lstr_literal.1,@object         # @str_literal.1
	.p2align	4
.Lstr_literal.1:
	.asciz	"With parentheses!"
	.size	.Lstr_literal.1, 18

	.section	".note.GNU-stack","",@progbits

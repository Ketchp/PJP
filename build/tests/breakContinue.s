	.text
	.file	"breakContinue"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	leaq	.Lstr_literal(%rip), %rdi
	callq	write_str@PLT
	movq	I@GOTPCREL(%rip), %rbx
	movl	$0, (%rbx)
	.p2align	4, 0x90
.LBB0_1:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_2
# %bb.6:                                # %else
                                        #   in Loop: Header=BB0_1 Depth=1
	cmpl	$3, (%rbx)
	je	.LBB0_2
# %bb.7:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	(%rbx)
	jmp	.LBB0_1
.LBB0_2:                                # %after
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.1(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	.p2align	4, 0x90
.LBB0_3:                                # %body9
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_4
# %bb.8:                                # %else19
                                        #   in Loop: Header=BB0_3 Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
.LBB0_4:                                # %stop10
                                        #   in Loop: Header=BB0_3 Depth=1
	cmpl	$3, (%rbx)
	je	.LBB0_9
# %bb.5:                                # %inc11
                                        #   in Loop: Header=BB0_3 Depth=1
	incl	(%rbx)
	jmp	.LBB0_3
.LBB0_9:                                # %after12
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.2(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	cmpl	$4, (%rbx)
	jg	.LBB0_12
	.p2align	4, 0x90
.LBB0_11:                               # %body30
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_12
# %bb.23:                               # %else40
                                        #   in Loop: Header=BB0_11 Depth=1
	incl	(%rbx)
	cmpl	$4, (%rbx)
	jle	.LBB0_11
.LBB0_12:                               # %after31
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.3(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	.p2align	4, 0x90
.LBB0_13:                               # %cond47
                                        # =>This Inner Loop Header: Depth=1
	cmpl	$4, (%rbx)
	jg	.LBB0_16
# %bb.14:                               # %body48
                                        #   in Loop: Header=BB0_13 Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	movl	(%rbx), %eax
	incl	%eax
	movl	%eax, (%rbx)
	cmpl	$2, %eax
	je	.LBB0_13
# %bb.15:                               # %else61
                                        #   in Loop: Header=BB0_13 Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	jmp	.LBB0_13
.LBB0_16:                               # %after49
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.4(%rip), %rdi
	callq	write_str@PLT
	movl	$1561, (%rbx)                   # imm = 0x619
	movl	$1561, %edi                     # imm = 0x619
	jmp	.LBB0_17
	.p2align	4, 0x90
.LBB0_20:                               # %else81
                                        #   in Loop: Header=BB0_17 Depth=1
	movl	(%rbx), %edi
	sarl	%edi
	movl	%edi, (%rbx)
.LBB0_17:                               # %cond74
                                        # =>This Inner Loop Header: Depth=1
	callq	writeln@PLT
	movl	(%rbx), %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	andl	$-2, %ecx
	subl	%ecx, %eax
	cmpl	$1, %eax
	jne	.LBB0_20
# %bb.18:                               # %after76
                                        #   in Loop: Header=BB0_17 Depth=1
	cmpl	$1, (%rbx)
	je	.LBB0_19
# %bb.22:                               # %else92
                                        #   in Loop: Header=BB0_17 Depth=1
	movl	(%rbx), %eax
	leal	1(%rax,%rax,2), %edi
	movl	%edi, (%rbx)
	jmp	.LBB0_17
.LBB0_19:                               # %after73
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	I,@object                       # @I
	.comm	I,4,4
	.type	.Lstr_literal,@object           # @str_literal
	.section	.rodata,"a",@progbits
.Lstr_literal:
	.asciz	"For loop break\n"
	.size	.Lstr_literal, 16

	.type	.Lstr_literal.1,@object         # @str_literal.1
	.p2align	4
.Lstr_literal.1:
	.asciz	"For loop continue\n"
	.size	.Lstr_literal.1, 19

	.type	.Lstr_literal.2,@object         # @str_literal.2
	.p2align	4
.Lstr_literal.2:
	.asciz	"While loop break\n"
	.size	.Lstr_literal.2, 18

	.type	.Lstr_literal.3,@object         # @str_literal.3
	.p2align	4
.Lstr_literal.3:
	.asciz	"While loop continue\n"
	.size	.Lstr_literal.3, 21

	.type	.Lstr_literal.4,@object         # @str_literal.4
	.p2align	4
.Lstr_literal.4:
	.asciz	"Nested break/continue\n"
	.size	.Lstr_literal.4, 23

	.section	".note.GNU-stack","",@progbits

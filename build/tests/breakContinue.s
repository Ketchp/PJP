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
	cmpl	$3, (%rbx)
	je	.LBB0_3
# %bb.2:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	(%rbx)
	jmp	.LBB0_1
.LBB0_3:                                # %after
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.1(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	.p2align	4, 0x90
.LBB0_4:                                # %body9
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_5
# %bb.8:                                # %else19
                                        #   in Loop: Header=BB0_4 Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$3, (%rbx)
	je	.LBB0_5
# %bb.9:                                # %inc11
                                        #   in Loop: Header=BB0_4 Depth=1
	incl	(%rbx)
	jmp	.LBB0_4
.LBB0_5:                                # %after12
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.2(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	cmpl	$4, (%rbx)
	jg	.LBB0_10
	.p2align	4, 0x90
.LBB0_7:                                # %body30
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$4, (%rbx)
	jle	.LBB0_7
.LBB0_10:                               # %after31
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.3(%rip), %rdi
	callq	write_str@PLT
	movl	$0, (%rbx)
	cmpl	$4, (%rbx)
	jg	.LBB0_13
	.p2align	4, 0x90
.LBB0_12:                               # %body47
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_13
# %bb.18:                               # %else58
                                        #   in Loop: Header=BB0_12 Depth=1
	movl	(%rbx), %edi
	callq	writeln@PLT
	cmpl	$4, (%rbx)
	jle	.LBB0_12
.LBB0_13:                               # %after48
	movl	(%rbx), %edi
	callq	writeln@PLT
	leaq	.Lstr_literal.4(%rip), %rdi
	callq	write_str@PLT
	movl	$1561, (%rbx)                   # imm = 0x619
	.p2align	4, 0x90
.LBB0_14:                               # %cond66
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_15 Depth 2
	movl	(%rbx), %edi
	callq	writeln@PLT
	.p2align	4, 0x90
.LBB0_15:                               # %cond71
                                        #   Parent Loop BB0_14 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	(%rbx), %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	andl	$-2, %ecx
	subl	%ecx, %eax
	cmpl	$1, %eax
	je	.LBB0_15
# %bb.16:                               # %else78
                                        #   in Loop: Header=BB0_14 Depth=1
	movl	(%rbx), %edi
	sarl	%edi
	movl	%edi, (%rbx)
	callq	writeln@PLT
	cmpl	$1, (%rbx)
	je	.LBB0_14
# %bb.17:                               # %else89
                                        #   in Loop: Header=BB0_14 Depth=1
	movl	(%rbx), %eax
	leal	1(%rax,%rax,2), %eax
	movl	%eax, (%rbx)
	jmp	.LBB0_14
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

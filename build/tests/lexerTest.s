	.text
	.file	"lexerTest"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	A@GOTPCREL(%rip), %rbx
	movl	$0, (%rbx)
	xorl	%edi, %edi
	callq	writeln@PLT
	movl	$100, (%rbx)
	movl	$100, %edi
	callq	writeln@PLT
	movl	$64, (%rbx)
	movl	$64, %edi
	callq	writeln@PLT
	movl	$256, (%rbx)                    # imm = 0x100
	movl	$256, %edi                      # imm = 0x100
	callq	writeln@PLT
	movq	B@GOTPCREL(%rip), %rax
	movabsq	$4672446955123638272, %rcx      # imm = 0x40D7DDC000000000
	movq	%rcx, (%rax)
	leaq	.Lstr_literal(%rip), %rdi
	movq	C@GOTPCREL(%rip), %rbx
	movq	%rdi, (%rbx)
	callq	write_str@PLT
	leaq	.Lstr_literal.1(%rip), %rdi
	callq	write_str@PLT
	leaq	.Lstr_literal.2(%rip), %rdi
	movq	%rdi, (%rbx)
	callq	write_str@PLT
	leaq	.Lstr_literal.3(%rip), %rdi
	callq	write_str@PLT
	leaq	.Lstr_literal.4(%rip), %rdi
	movq	%rdi, (%rbx)
	callq	write_str@PLT
	leaq	.Lstr_literal.5(%rip), %rdi
	callq	write_str@PLT
	leaq	.Lstr_literal.10(%rip), %rdi
	movq	%rdi, (%rbx)
	callq	write_str@PLT
	leaq	.Lstr_literal.11(%rip), %rdi
	callq	write_str@PLT
	leaq	.Lstr_literal.21(%rip), %rax
	movq	%rax, (%rbx)
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	A,@object                       # @A
	.comm	A,4,4
	.type	B,@object                       # @B
	.comm	B,8,8
	.type	C,@object                       # @C
	.comm	C,8,8
	.type	.Lstr_literal,@object           # @str_literal
	.section	.rodata,"a",@progbits
.Lstr_literal:
	.asciz	"fwrgdbf"
	.size	.Lstr_literal, 8

	.type	.Lstr_literal.1,@object         # @str_literal.1
.Lstr_literal.1:
	.asciz	"\n"
	.size	.Lstr_literal.1, 2

	.type	.Lstr_literal.2,@object         # @str_literal.2
.Lstr_literal.2:
	.zero	1
	.size	.Lstr_literal.2, 1

	.type	.Lstr_literal.3,@object         # @str_literal.3
.Lstr_literal.3:
	.asciz	"\n"
	.size	.Lstr_literal.3, 2

	.type	.Lstr_literal.4,@object         # @str_literal.4
.Lstr_literal.4:
	.asciz	"'"
	.size	.Lstr_literal.4, 2

	.type	.Lstr_literal.5,@object         # @str_literal.5
.Lstr_literal.5:
	.asciz	"\n"
	.size	.Lstr_literal.5, 2

	.type	.Lstr_literal.6,@object         # @str_literal.6
.Lstr_literal.6:
	.asciz	"\""
	.size	.Lstr_literal.6, 2

	.type	.Lstr_literal.7,@object         # @str_literal.7
.Lstr_literal.7:
	.asciz	"\nabc"
	.size	.Lstr_literal.7, 5

	.type	.Lstr_literal.8,@object         # @str_literal.8
.Lstr_literal.8:
	.asciz	"abc"
	.size	.Lstr_literal.8, 4

	.type	.Lstr_literal.9,@object         # @str_literal.9
.Lstr_literal.9:
	.asciz	"\\"
	.size	.Lstr_literal.9, 2

	.type	.Lstr_literal.10,@object        # @str_literal.10
.Lstr_literal.10:
	.asciz	"new\nline"
	.size	.Lstr_literal.10, 9

	.type	.Lstr_literal.11,@object        # @str_literal.11
.Lstr_literal.11:
	.asciz	"\n"
	.size	.Lstr_literal.11, 2

	.type	.Lstr_literal.12,@object        # @str_literal.12
.Lstr_literal.12:
	.asciz	"?"
	.size	.Lstr_literal.12, 2

	.type	.Lstr_literal.13,@object        # @str_literal.13
.Lstr_literal.13:
	.zero	2
	.size	.Lstr_literal.13, 2

	.type	.Lstr_literal.14,@object        # @str_literal.14
.Lstr_literal.14:
	.asciz	"abc\\qdef"
	.size	.Lstr_literal.14, 9

	.type	.Lstr_literal.15,@object        # @str_literal.15
.Lstr_literal.15:
	.asciz	"_\017_"
	.size	.Lstr_literal.15, 4

	.type	.Lstr_literal.16,@object        # @str_literal.16
.Lstr_literal.16:
	.asciz	"@AB"
	.size	.Lstr_literal.16, 4

	.type	.Lstr_literal.17,@object        # @str_literal.17
.Lstr_literal.17:
	.asciz	"\017"
	.size	.Lstr_literal.17, 2

	.type	.Lstr_literal.18,@object        # @str_literal.18
.Lstr_literal.18:
	.asciz	"\0000a"
	.size	.Lstr_literal.18, 4

	.type	.Lstr_literal.19,@object        # @str_literal.19
.Lstr_literal.19:
	.asciz	"\0000"
	.size	.Lstr_literal.19, 3

	.type	.Lstr_literal.20,@object        # @str_literal.20
.Lstr_literal.20:
	.asciz	"S0"
	.size	.Lstr_literal.20, 3

	.type	.Lstr_literal.21,@object        # @str_literal.21
.Lstr_literal.21:
	.asciz	"(9"
	.size	.Lstr_literal.21, 3

	.section	".note.GNU-stack","",@progbits

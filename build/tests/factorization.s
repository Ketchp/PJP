	.text
	.file	"factorization"
	.globl	factorization                   # -- Begin function factorization
	.p2align	4, 0x90
	.type	factorization,@function
factorization:                          # @factorization
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, 12(%rsp)
	leaq	.Lstr_literal(%rip), %rdi
	callq	write_str@PLT
	movl	12(%rsp), %edi
	callq	writeln@PLT
	cmpl	$3, 12(%rsp)
	jg	.LBB0_1
# %bb.16:                               # %body
	movl	12(%rsp), %edi
	callq	writeln@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
	.p2align	4, 0x90
.LBB0_2:                                # %body5
                                        #   in Loop: Header=BB0_1 Depth=1
	.cfi_def_cfa_offset 32
	movl	$2, %edi
	callq	writeln@PLT
	sarl	12(%rsp)
.LBB0_1:                                # %cond
                                        # =>This Inner Loop Header: Depth=1
	movl	12(%rsp), %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	andl	$-2, %ecx
	cmpl	%ecx, %eax
	je	.LBB0_2
	jmp	.LBB0_3
	.p2align	4, 0x90
.LBB0_4:                                # %body12
                                        #   in Loop: Header=BB0_3 Depth=1
	movl	$3, %edi
	callq	writeln@PLT
	imull	$-1431655765, 12(%rsp), %eax    # imm = 0xAAAAAAAB
	movl	%eax, 12(%rsp)
.LBB0_3:                                # %cond11
                                        # =>This Inner Loop Header: Depth=1
	imull	$-1431655765, 12(%rsp), %eax    # imm = 0xAAAAAAAB
	addl	$715827882, %eax                # imm = 0x2AAAAAAA
	cmpl	$1431655764, %eax               # imm = 0x55555554
	jbe	.LBB0_4
# %bb.5:                                # %after13
	movl	12(%rsp), %eax
	movl	%eax, 20(%rsp)
	movl	$5, 16(%rsp)
	jmp	.LBB0_6
	.p2align	4, 0x90
.LBB0_15:                               # %after44
                                        #   in Loop: Header=BB0_6 Depth=1
	addl	$4, 16(%rsp)
.LBB0_6:                                # %cond22
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_10 Depth 2
                                        #     Child Loop BB0_13 Depth 2
	movl	16(%rsp), %eax
	cmpl	20(%rsp), %eax
	jg	.LBB0_7
	.p2align	4, 0x90
.LBB0_10:                               # %cond28
                                        #   Parent Loop BB0_6 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	12(%rsp), %eax
	cltd
	idivl	16(%rsp)
	testl	%edx, %edx
	jne	.LBB0_12
# %bb.11:                               # %body29
                                        #   in Loop: Header=BB0_10 Depth=2
	movl	16(%rsp), %edi
	callq	writeln@PLT
	movl	12(%rsp), %eax
	cltd
	idivl	16(%rsp)
	movl	%eax, 12(%rsp)
	jmp	.LBB0_10
	.p2align	4, 0x90
.LBB0_12:                               # %after30
                                        #   in Loop: Header=BB0_6 Depth=1
	addl	$2, 16(%rsp)
	.p2align	4, 0x90
.LBB0_13:                               # %cond42
                                        #   Parent Loop BB0_6 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	12(%rsp), %eax
	cltd
	idivl	16(%rsp)
	testl	%edx, %edx
	jne	.LBB0_15
# %bb.14:                               # %body43
                                        #   in Loop: Header=BB0_13 Depth=2
	movl	16(%rsp), %edi
	callq	writeln@PLT
	movl	12(%rsp), %eax
	cltd
	idivl	16(%rsp)
	movl	%eax, 12(%rsp)
	jmp	.LBB0_13
.LBB0_7:                                # %after24
	cmpl	$1, 12(%rsp)
	je	.LBB0_9
# %bb.8:                                # %body59
	movl	12(%rsp), %edi
	callq	writeln@PLT
.LBB0_9:                                # %after61
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	factorization, .Lfunc_end0-factorization
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
	xorl	%edi, %edi
	callq	factorization@PLT
	movl	$1, %edi
	callq	factorization@PLT
	movl	$2, %edi
	callq	factorization@PLT
	movl	$3, %edi
	callq	factorization@PLT
	movl	$4, %edi
	callq	factorization@PLT
	movl	$5, %edi
	callq	factorization@PLT
	movl	$6, %edi
	callq	factorization@PLT
	movl	$7, %edi
	callq	factorization@PLT
	movl	$8, %edi
	callq	factorization@PLT
	movl	$9, %edi
	callq	factorization@PLT
	movl	$10, %edi
	callq	factorization@PLT
	movl	$11, %edi
	callq	factorization@PLT
	movl	$12, %edi
	callq	factorization@PLT
	movl	$13, %edi
	callq	factorization@PLT
	movl	$14, %edi
	callq	factorization@PLT
	movl	$15, %edi
	callq	factorization@PLT
	movl	$16, %edi
	callq	factorization@PLT
	movl	$17, %edi
	callq	factorization@PLT
	movl	$100, %edi
	callq	factorization@PLT
	movl	$131, %edi
	callq	factorization@PLT
	movl	$133, %edi
	callq	factorization@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.Lstr_literal,@object           # @str_literal
	.section	.rodata,"a",@progbits
.Lstr_literal:
	.asciz	"Factors of: "
	.size	.Lstr_literal, 13

	.section	".note.GNU-stack","",@progbits
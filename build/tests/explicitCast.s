	.text
	.file	"explicitCast"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function write_real
.LCPI0_0:
	.quad	0x4024000000000000              # double 10
	.text
	.globl	write_real
	.p2align	4, 0x90
	.type	write_real,@function
write_real:                             # @write_real
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movsd	%xmm0, 8(%rsp)
	cvttsd2si	%xmm0, %edi
	callq	write@PLT
	movsd	8(%rsp), %xmm0                  # xmm0 = mem[0],zero
	cvttpd2dq	%xmm0, %xmm1
	cvtdq2pd	%xmm1, %xmm1
	subsd	%xmm1, %xmm0
	movsd	%xmm0, 8(%rsp)
	leaq	.Lstr_literal(%rip), %rdi
	callq	write_str@PLT
	movl	$0, 20(%rsp)
	.p2align	4, 0x90
.LBB0_1:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movsd	8(%rsp), %xmm0                  # xmm0 = mem[0],zero
	mulsd	.LCPI0_0(%rip), %xmm0
	movsd	%xmm0, 8(%rsp)
	cvttsd2si	%xmm0, %edi
	callq	write@PLT
	movsd	8(%rsp), %xmm0                  # xmm0 = mem[0],zero
	cvttpd2dq	%xmm0, %xmm1
	cvtdq2pd	%xmm1, %xmm1
	subsd	%xmm1, %xmm0
	movsd	%xmm0, 8(%rsp)
	cmpl	$2, 20(%rsp)
	je	.LBB0_3
# %bb.2:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	20(%rsp)
	jmp	.LBB0_1
.LBB0_3:                                # %after
	leaq	.Lstr_literal.1(%rip), %rdi
	callq	write_str@PLT
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	write_real, .Lfunc_end0-write_real
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function main
.LCPI1_0:
	.quad	0x4014000000000000              # double 5
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	i@GOTPCREL(%rip), %r14
	movl	$5, (%r14)
	movq	r@GOTPCREL(%rip), %rbx
	movabsq	$4617540697942969549, %rax      # imm = 0x4014CCCCCCCCCCCD
	movq	%rax, (%rbx)
	movsd	.LCPI1_0(%rip), %xmm0           # xmm0 = mem[0],zero
	callq	write_real@PLT
	movsd	(%rbx), %xmm0                   # xmm0 = mem[0],zero
	callq	write_real@PLT
	movl	(%r14), %edi
	callq	writeln@PLT
	cvttsd2si	(%rbx), %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	i,@object                       # @i
	.comm	i,4,4
	.type	r,@object                       # @r
	.comm	r,8,8
	.type	.Lstr_literal,@object           # @str_literal
	.section	.rodata,"a",@progbits
.Lstr_literal:
	.asciz	"."
	.size	.Lstr_literal, 2

	.type	.Lstr_literal.1,@object         # @str_literal.1
.Lstr_literal.1:
	.asciz	"\n"
	.size	.Lstr_literal.1, 2

	.section	".note.GNU-stack","",@progbits

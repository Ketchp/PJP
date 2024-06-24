	.text
	.file	"arrayAverage"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	I@GOTPCREL(%rip), %rbx
	movl	$-20, (%rbx)
	movq	X@GOTPCREL(%rip), %r12
	.p2align	4, 0x90
.LBB0_1:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %eax
	leal	20(%rax), %ecx
	movslq	%ecx, %rcx
	movl	$0, (%r12,%rcx,4)
	cmpl	$20, %eax
	je	.LBB0_3
# %bb.2:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	(%rbx)
	jmp	.LBB0_1
.LBB0_3:                                # %after
	movq	NUM@GOTPCREL(%rip), %r14
	movq	%r14, %rdi
	callq	readln@PLT
	movl	$0, (%rbx)
	movq	TEMP@GOTPCREL(%rip), %r15
	.p2align	4, 0x90
.LBB0_4:                                # %body2
                                        # =>This Inner Loop Header: Depth=1
	movq	%r15, %rdi
	callq	readln@PLT
	movl	(%r15), %eax
	addl	$20, %eax
	cltq
	incl	(%r12,%rax,4)
	movl	(%r14), %eax
	decl	%eax
	cmpl	%eax, (%rbx)
	je	.LBB0_6
# %bb.5:                                # %inc3
                                        #   in Loop: Header=BB0_4 Depth=1
	incl	(%rbx)
	jmp	.LBB0_4
.LBB0_6:                                # %after4
	movq	SUM@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	movl	$20, (%rbx)
	.p2align	4, 0x90
.LBB0_7:                                # %body18
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rbx), %ecx
	leal	20(%rcx), %edx
	movslq	%edx, %rdx
	movl	(%r12,%rdx,4), %edx
	imull	%ecx, %edx
	addl	%edx, (%rax)
	cmpl	$-20, %ecx
	je	.LBB0_9
# %bb.8:                                # %inc19
                                        #   in Loop: Header=BB0_7 Depth=1
	decl	(%rbx)
	jmp	.LBB0_7
.LBB0_9:                                # %after20
	movl	(%rax), %eax
	cltd
	idivl	(%r14)
	movl	%eax, %edi
	callq	writeln@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	I,@object                       # @I
	.comm	I,4,4
	.type	NUM,@object                     # @NUM
	.comm	NUM,4,4
	.type	SUM,@object                     # @SUM
	.comm	SUM,4,4
	.type	TEMP,@object                    # @TEMP
	.comm	TEMP,4,4
	.type	X,@object                       # @X
	.comm	X,164,16
	.section	".note.GNU-stack","",@progbits

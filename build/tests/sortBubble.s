	.text
	.file	"sortBubble"
	.globl	main                            # -- Begin function main
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
	movq	I@GOTPCREL(%rip), %r14
	movl	$0, (%r14)
	movq	X@GOTPCREL(%rip), %rbx
	.p2align	4, 0x90
.LBB0_1:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movslq	(%r14), %rax
	movl	$20, %ecx
	subl	%eax, %ecx
	cmpq	$20, %rax
	movl	%ecx, (%rbx,%rax,4)
	je	.LBB0_3
# %bb.2:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	(%r14)
	jmp	.LBB0_1
.LBB0_3:                                # %after
	movl	$0, (%r14)
	.p2align	4, 0x90
.LBB0_4:                                # %body3
                                        # =>This Inner Loop Header: Depth=1
	movslq	(%r14), %rax
	movl	(%rbx,%rax,4), %edi
	callq	writeln@PLT
	cmpl	$20, (%r14)
	je	.LBB0_6
# %bb.5:                                # %inc4
                                        #   in Loop: Header=BB0_4 Depth=1
	incl	(%r14)
	jmp	.LBB0_4
.LBB0_6:                                # %after5
	movl	$1, (%r14)
	movq	J@GOTPCREL(%rip), %rax
	movq	TEMP@GOTPCREL(%rip), %r8
	.p2align	4, 0x90
.LBB0_7:                                # %body13
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_8 Depth 2
	movl	$20, (%rax)
	.p2align	4, 0x90
.LBB0_8:                                # %body16
                                        #   Parent Loop BB0_7 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movslq	(%rax), %rdx
	movl	(%rbx,%rdx,4), %esi
	decl	%edx
	movslq	%edx, %rdx
	cmpl	(%rbx,%rdx,4), %esi
	jge	.LBB0_10
# %bb.9:                                # %body29
                                        #   in Loop: Header=BB0_8 Depth=2
	movl	(%rax), %edx
	decl	%edx
	movslq	%edx, %rdx
	movl	(%rbx,%rdx,4), %edx
	movl	%edx, (%r8)
	movslq	(%rax), %rsi
	leal	-1(%rsi), %edi
	movslq	%edi, %rdi
	movl	(%rbx,%rsi,4), %ecx
	movl	%ecx, (%rbx,%rdi,4)
	movl	%edx, (%rbx,%rsi,4)
.LBB0_10:                               # %after30
                                        #   in Loop: Header=BB0_8 Depth=2
	movl	(%rax), %ecx
	cmpl	(%r14), %ecx
	je	.LBB0_11
# %bb.13:                               # %inc17
                                        #   in Loop: Header=BB0_8 Depth=2
	decl	(%rax)
	jmp	.LBB0_8
	.p2align	4, 0x90
.LBB0_11:                               # %after18
                                        #   in Loop: Header=BB0_7 Depth=1
	cmpl	$20, (%r14)
	je	.LBB0_14
# %bb.12:                               # %inc14
                                        #   in Loop: Header=BB0_7 Depth=1
	incl	(%r14)
	jmp	.LBB0_7
.LBB0_14:                               # %after15
	movl	$0, (%r14)
	.p2align	4, 0x90
.LBB0_15:                               # %body56
                                        # =>This Inner Loop Header: Depth=1
	movslq	(%r14), %rax
	movl	(%rbx,%rax,4), %edi
	callq	writeln@PLT
	cmpl	$20, (%r14)
	je	.LBB0_17
# %bb.16:                               # %inc57
                                        #   in Loop: Header=BB0_15 Depth=1
	incl	(%r14)
	jmp	.LBB0_15
.LBB0_17:                               # %after58
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	I,@object                       # @I
	.comm	I,4,4
	.type	J,@object                       # @J
	.comm	J,4,4
	.type	TEMP,@object                    # @TEMP
	.comm	TEMP,4,4
	.type	X,@object                       # @X
	.comm	X,84,16
	.section	".note.GNU-stack","",@progbits

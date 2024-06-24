	.text
	.file	"arrayMax"
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
	movq	X@GOTPCREL(%rip), %r14
	movabsq	$283467841547, %rax             # imm = 0x420000000B
	movq	%rax, (%r14)
	movabsq	$210453397632, %rax             # imm = 0x3100000080
	movq	%rax, 8(%r14)
	movabsq	$197568495749, %rax             # imm = 0x2E00000085
	movq	%rax, 16(%r14)
	movabsq	$373662154767, %rax             # imm = 0x570000000F
	movq	%rax, 24(%r14)
	movabsq	$158913790007, %rax             # imm = 0x2500000037
	movq	%rax, 32(%r14)
	movabsq	$188978561102, %rax             # imm = 0x2C0000004E
	movq	%rax, 40(%r14)
	movabsq	$163208757281, %rax             # imm = 0x2600000021
	movq	%rax, 48(%r14)
	movabsq	$25769803861, %rax              # imm = 0x600000055
	movq	%rax, 56(%r14)
	movabsq	$17179869334, %rax              # imm = 0x400000096
	movq	%rax, 64(%r14)
	movabsq	$236223201281, %rax             # imm = 0x3700000001
	movq	%rax, 72(%r14)
	movl	$78, 80(%r14)
	movq	I@GOTPCREL(%rip), %rbx
	movl	$0, (%rbx)
	.p2align	4, 0x90
.LBB0_1:                                # %body
                                        # =>This Inner Loop Header: Depth=1
	movslq	(%rbx), %rax
	movl	(%r14,%rax,4), %edi
	callq	writeln@PLT
	cmpl	$20, (%rbx)
	je	.LBB0_3
# %bb.2:                                # %inc
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	(%rbx)
	jmp	.LBB0_1
.LBB0_3:                                # %after
	movl	(%r14), %ecx
	movq	MAX@GOTPCREL(%rip), %rax
	movl	%ecx, (%rax)
	movl	$1, (%rbx)
	.p2align	4, 0x90
.LBB0_4:                                # %body4
                                        # =>This Inner Loop Header: Depth=1
	movl	(%rax), %ecx
	movslq	(%rbx), %rdx
	cmpl	(%r14,%rdx,4), %ecx
	jge	.LBB0_6
# %bb.5:                                # %body12
                                        #   in Loop: Header=BB0_4 Depth=1
	movslq	(%rbx), %rcx
	movl	(%r14,%rcx,4), %ecx
	movl	%ecx, (%rax)
.LBB0_6:                                # %after13
                                        #   in Loop: Header=BB0_4 Depth=1
	cmpl	$20, (%rbx)
	je	.LBB0_8
# %bb.7:                                # %inc5
                                        #   in Loop: Header=BB0_4 Depth=1
	incl	(%rbx)
	jmp	.LBB0_4
.LBB0_8:                                # %after6
	movl	(%rax), %edi
	callq	writeln@PLT
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
	.type	MAX,@object                     # @MAX
	.comm	MAX,4,4
	.type	X,@object                       # @X
	.comm	X,84,16
	.section	".note.GNU-stack","",@progbits

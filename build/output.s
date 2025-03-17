	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"my_module"
	.def	arthimatic;
	.scl	2;
	.type	32;
	.endef
	.globl	arthimatic                      # -- Begin function arthimatic
	.p2align	4, 0x90
arthimatic:                             # @arthimatic
.seh_proc arthimatic
# %bb.0:                                # %entry
	subq	$24, %rsp
	.seh_stackalloc 24
	.seh_endprologue
	movq	%r8, 48(%rsp)
	movq	%r9, 56(%rsp)
	leaq	48(%rsp), %rax
	movq	%rax, 8(%rsp)
	movl	$0, (%rsp)
	movl	%ecx, 4(%rsp)
	movl	%edx, 16(%rsp)
	cmpl	$2, %edx
	je	.LBB0_7
# %bb.1:                                # %entry
	cmpl	$3, %edx
	jne	.LBB0_2
# %bb.8:                                # %DivIntialize
	movq	8(%rsp), %rax
	addq	$7, %rax
	andq	$-8, %rax
	leaq	8(%rax), %rcx
	movq	%rcx, 8(%rsp)
	movsd	(%rax), %xmm0                   # xmm0 = mem[0],zero
	cvtsd2ss	%xmm0, %xmm0
	decl	4(%rsp)
	movss	%xmm0, (%rsp)
	jmp	.LBB0_2
.LBB0_7:                                # %MulIntialize
	movl	$1065353216, (%rsp)             # imm = 0x3F800000
	jmp	.LBB0_2
	.p2align	4, 0x90
.LBB0_5:                                # %Mul
                                        #   in Loop: Header=BB0_2 Depth=1
	mulss	%xmm1, %xmm0
.LBB0_10:                               # %end1
                                        #   in Loop: Header=BB0_2 Depth=1
	movss	%xmm0, (%rsp)
	decl	%eax
	movl	%eax, 4(%rsp)
	je	.LBB0_11
.LBB0_2:                                # %loop
                                        # =>This Inner Loop Header: Depth=1
	movl	4(%rsp), %eax
	movq	8(%rsp), %rcx
	addq	$7, %rcx
	andq	$-8, %rcx
	leaq	8(%rcx), %r8
	movq	%r8, 8(%rsp)
	cmpl	$3, %edx
	ja	.LBB0_11
# %bb.3:                                # %loop
                                        #   in Loop: Header=BB0_2 Depth=1
	movsd	(%rcx), %xmm0                   # xmm0 = mem[0],zero
	xorps	%xmm1, %xmm1
	cvtsd2ss	%xmm0, %xmm1
	movss	(%rsp), %xmm0                   # xmm0 = mem[0],zero,zero,zero
	movl	%edx, %ecx
	leaq	.LJTI0_0(%rip), %r8
	movslq	(%r8,%rcx,4), %rcx
	addq	%r8, %rcx
	jmpq	*%rcx
.LBB0_9:                                # %Add
                                        #   in Loop: Header=BB0_2 Depth=1
	addss	%xmm1, %xmm0
	jmp	.LBB0_10
	.p2align	4, 0x90
.LBB0_6:                                # %Div
                                        #   in Loop: Header=BB0_2 Depth=1
	divss	%xmm1, %xmm0
	jmp	.LBB0_10
	.p2align	4, 0x90
.LBB0_4:                                # %Sub
                                        #   in Loop: Header=BB0_2 Depth=1
	subss	%xmm1, %xmm0
	jmp	.LBB0_10
.LBB0_11:                               # %end
	movss	(%rsp), %xmm0                   # xmm0 = mem[0],zero,zero,zero
	addq	$24, %rsp
	retq
	.section	.rdata,"dr"
	.p2align	2, 0x0
.LJTI0_0:
	.long	.LBB0_9-.LJTI0_0
	.long	.LBB0_4-.LJTI0_0
	.long	.LBB0_5-.LJTI0_0
	.long	.LBB0_6-.LJTI0_0
	.text
	.seh_endproc
                                        # -- End function
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"dr"
	.p2align	3, 0x0                          # -- Begin function main
.LCPI1_0:
	.quad	0x4008000000000000              # double 3
.LCPI1_1:
	.quad	0x4010000000000000              # double 4
	.text
	.globl	main
	.p2align	4, 0x90
main:                                   # @main
.seh_proc main
# %bb.0:                                # %entry
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$48, %rsp
	.seh_stackalloc 48
	leaq	48(%rsp), %rbp
	.seh_setframe %rbp, 48
	.seh_endprologue
	callq	__main
	leaq	.Lhello_world(%rip), %rcx
	callq	printf
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, 40(%rsp)
	movabsq	$4619567317775286272, %rax      # imm = 0x401C000000000000
	movq	%rax, 32(%rsp)
	movq	.LCPI1_0(%rip), %xmm2           # xmm2 = [3.0E+0,0.0E+0]
	movq	.LCPI1_1(%rip), %xmm3           # xmm3 = [4.0E+0,0.0E+0]
	movl	$4, %ecx
	xorl	%edx, %edx
	movq	%xmm2, %r8
	movq	%xmm3, %r9
	callq	arthimatic
	cvtss2sd	%xmm0, %xmm1
	leaq	.Lformat_str(%rip), %rcx
	movq	%xmm1, %rdx
	callq	printf
	nop
	addq	$48, %rsp
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
	.section	.rdata,"dr"
.Lformat_str:                           # @format_str
	.asciz	"%f"

.Lhello_world:                          # @hello_world
	.asciz	"Hello, World!\n"


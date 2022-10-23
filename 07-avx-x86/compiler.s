	.file	"compiler.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	sum_0
	.type	sum_0, @function
sum_0:
.LFB5307:
	.cfi_startproc
	endbr64
	lea	rax, 15[rdi]
	sub	rax, rdx
	cmp	rax, 30
	jbe	.L2
	lea	rax, 15[rsi]
	sub	rax, rdx
	cmp	rax, 30
	jbe	.L2
	movdqu	xmm0, XMMWORD PTR [rdi]
	movdqu	xmm1, XMMWORD PTR [rsi]
	paddd	xmm0, xmm1
	movups	XMMWORD PTR [rdx], xmm0
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	mov	eax, DWORD PTR [rsi]
	add	eax, DWORD PTR [rdi]
	mov	DWORD PTR [rdx], eax
	mov	eax, DWORD PTR 4[rsi]
	add	eax, DWORD PTR 4[rdi]
	mov	DWORD PTR 4[rdx], eax
	mov	eax, DWORD PTR 8[rsi]
	add	eax, DWORD PTR 8[rdi]
	mov	DWORD PTR 8[rdx], eax
	mov	eax, DWORD PTR 12[rsi]
	add	eax, DWORD PTR 12[rdi]
	mov	DWORD PTR 12[rdx], eax
	ret
	.cfi_endproc
.LFE5307:
	.size	sum_0, .-sum_0
	.p2align 4
	.globl	sum_1
	.type	sum_1, @function
sum_1:
.LFB5308:
	.cfi_startproc
	endbr64
	movdqu	xmm0, XMMWORD PTR [rsi]
	movdqu	xmm1, XMMWORD PTR [rdi]
	paddd	xmm0, xmm1
	movups	XMMWORD PTR [rdx], xmm0
	ret
	.cfi_endproc
.LFE5308:
	.size	sum_1, .-sum_1
	.p2align 4
	.globl	sum_2
	.type	sum_2, @function
sum_2:
.LFB5309:
	.cfi_startproc
	endbr64
	movdqa	xmm0, XMMWORD PTR [rsi]
	paddd	xmm0, XMMWORD PTR [rdi]
	movaps	XMMWORD PTR [rdx], xmm0
	ret
	.cfi_endproc
.LFE5309:
	.size	sum_2, .-sum_2
	.p2align 4
	.globl	sum_3
	.type	sum_3, @function
sum_3:
.LFB5310:
	.cfi_startproc
	endbr64
	movdqa	xmm0, XMMWORD PTR [rdi]
	paddd	xmm0, XMMWORD PTR [rsi]
	movaps	XMMWORD PTR [rdx], xmm0
	ret
	.cfi_endproc
.LFE5310:
	.size	sum_3, .-sum_3
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:

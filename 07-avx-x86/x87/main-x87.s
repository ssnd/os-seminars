	.file	"main.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	square
	.type	square, @function
square:
.LFB23:
	.cfi_startproc
	endbr64
	movsd	QWORD PTR -8[rsp], xmm0
	fld	QWORD PTR -8[rsp]
	fmul	st, st(0)
	fstp	QWORD PTR -8[rsp]
	movsd	xmm0, QWORD PTR -8[rsp]
	ret
	.cfi_endproc
.LFE23:
	.size	square, .-square
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"10*10=%f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	endbr64
	sub	rsp, 24
	.cfi_def_cfa_offset 32
	fld	DWORD PTR .LC1[rip]
	mov	edi, 1
	lea	rsi, .LC2[rip]
	mov	eax, 1
	fstp	QWORD PTR 8[rsp]
	movsd	xmm0, QWORD PTR 8[rsp]
	call	__printf_chk@PLT
	xor	eax, eax
	add	rsp, 24
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC1:
	.long	1120403456
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

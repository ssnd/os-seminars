.intel_syntax noprefix

.text
.globl func

.section .rodata

.str:
    .string "hello world\n"

.section .text

func:
    sub rsp, 8
    push 8


    pop rax
    add rsp, 8
    ret
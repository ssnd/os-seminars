.intel_syntax noprefix
.text
.globl main
.section .rodata

.str:
    .string "Hello world\n"

.section .text

main:
    sub rsp, 8 // do we need this here?
    lea rdi, .str[rip] // same as [.str+rip]

    call printf

    add rsp, 8 // and this?
    ret
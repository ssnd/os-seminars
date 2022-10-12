.intel_syntax noprefix
.text
.globl main
.section .rodata

.str:
    .string "Args: %d %d %d %d %d %d %d %d\n"

.section .text

/*
Arg register names: `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9`
 */

main:
    enter 0,0

    lea rdi, .str[rip] // same as [.str+rip]
    mov rsi, 1
    mov rdx, 2
    mov rcx, 3
    mov r8, 4
    mov r9, 5

    sub rsp, 8
    push 8  // same as:
            // sub rsp, 8
            // mov DWORD PTR [rsp], 8
    push 7
    push 6

    call printf

    add rsp, 32

    leave
    ret
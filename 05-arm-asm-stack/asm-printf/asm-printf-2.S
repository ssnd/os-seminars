.section .text

.global main

main:
    sub sp, sp, #32

    // put 8 & 9 on the stack
    mov x0, #8
    str x0, [sp]

    mov x0, #9
    str x0, [sp, #8]

    str x30, [sp, #16]

    adr x0, fmt_str
    mov x1, 1
    mov x2, 2
    mov x3, 3
    mov x4, 4
    mov x5, 5
    mov x6, 6
    mov x7, 7

    bl printf

    ldr x30, [sp, #16]
    add sp, sp, #32
    ret

.section .data 
fmt_str:
     .string "%d %d %d %d %d %d %d %d %d\n"

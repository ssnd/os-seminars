.section .text

.global main

main:
    sub sp, sp, #16
    str x30, [sp]

    adr x0, fmt_str
    mov x1, #100

    bl printf

    // ldr x30, [sp]
    add sp, sp, #16
    ret

.section .data 
fmt_str:
     .string "%d\n"
.section .text

.global func1

func1:
    sub sp, sp, #32

    add x0, x0, #1
    str x0, [sp]

    add x0, x0, #1
    str x0, [sp, #8]

    add x0, x0, #1
    str x0, [sp, #16]

    add x0, x0, #1
    str x0, [sp, #24]

    add sp, sp, #32
    ret
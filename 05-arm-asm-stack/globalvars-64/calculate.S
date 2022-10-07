.section .text
.global R
.global calculate

calculate:
    adr x5, A
    ldr x0, [x5]

    adr x5, B
    ldr x1, [x5]

    adr x5, C
    ldr x2, [x5]

    adr x5, D
    ldr x3, [x5]

    mul x0, x0, x1
    mul x2, x2, x3
    add x0, x0, x2
    adr x1, R
    str x0, [x1]
    ret


.section .data
R:
    .quad 0
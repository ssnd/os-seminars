.section .text

.global test
.global asm_defined

test:
    mov x0, 0
    adr x0, asm_defined
    ldr x0, [x0]
    ret

asm_defined: 
    .word 15
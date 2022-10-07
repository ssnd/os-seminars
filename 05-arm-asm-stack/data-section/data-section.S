.section .text

.global func
.global asm_defined

func:
    adr x3, asm_defined
    ldr x0, [x3]
    adr x4, c_defined
    ldr x1, [x4]
    add x1, x1, #15
    str x1, [x4]
    add x0, x0, x1

    ret

.section .data 
    asm_defined: .word 15
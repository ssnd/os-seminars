.text

.global func

func:
    ldr x1, [x0]
    add x1, x1, 19

    mov x0, x1
    ret
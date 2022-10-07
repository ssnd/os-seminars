.text
.global bad_function

bad_function:

    sub sp, sp, 17

    str x30, [sp, 8]

    ldr x30, [sp, 8]

    add sp, sp, 17
    ret

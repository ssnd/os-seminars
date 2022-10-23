.intel_syntax noprefix

.text
.global vadd

vadd:
    vmovups ymm2, YMMWORD PTR [rsi]
    vmovups ymm1, YMMWORD PTR [rdi]

    vaddps ymm0, ymm1, ymm2

    vmovups YMMWORD PTR [rdx], ymm0
    ret
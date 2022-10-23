.intel_syntax noprefix
.section .text
.global some_func 

some_func:
    addps xmm0, xmm1
    addps xmm0, xmm2
    addps xmm0, xmm3
    addps xmm0, xmm4
    addps xmm0, xmm5
    addps xmm0, xmm6
    addps xmm0, xmm7

    sub rsp, 16
    movdqu XMMWORD PTR [rsp], xmm1
    addps xmm0, xmm1
    add rsp, 16

    ret

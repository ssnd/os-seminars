.intel_syntax noprefix
.section .text
.global very_important_function 

very_important_function:
    xor rax, rax                    // rax=0
    vxorpd ymm2, ymm2, ymm2         // ymm2=0, use ymm2 as an accumulator

    .loop:
    vmovaps ymm0, [rsi + rax*4]     // load the values from the 2nd and 3rd arguments
    vmovaps ymm1, [rdx + rax*4]     // to ymm0 and ymm1 registers

    vaddps ymm0, ymm0, ymm1         // ymm0+=ymm1
    vaddps ymm2, ymm2, ymm0         // ymm2+=ymm0
    vmovaps [rcx + rax*4], ymm0 

    add rax, 8                      // add 8 to our counter as we're processing 8 floats at a time

    cmp rax, rdi                    // if rax==rdi, exit the loop
    jnz .loop

    vmovaps ymm0, ymm2 // ymm0={0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10},

    vextractf128 xmm1, ymm0, 1 // xmm1={0xa, 0xc, 0xe, 0x10}, xmm0={0x2, 0x4, 0x6, 0x8}
    haddps xmm1, xmm0 // xmm1 = 0xa+0xc, 0xe+0x10, 0x2+0x4, 0x6+0x8
    haddps xmm1, xmm1 // xmm1 = 0xa+0xc+0xe+0x10, 0x2+0x4+0x6+0x8,  0xa+0xc+0xe+0x10, 0x2+0x4+0x6+0x8
    haddps xmm1, xmm1 // xmm1 = 0xa+0xc+0xe+0x10+0x2+0x4+0x6+0x8, ...

    vcvtsi2sd xmm0, xmm0, rdi  // the vcvtsi2sd and vcvtss2sd instructions 
    vcvtss2sd xmm1, xmm1, xmm1 // are used for type conversion

    vdivsd xmm0, xmm1, xmm0 

    ret
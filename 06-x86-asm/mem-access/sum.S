.intel_syntax noprefix
.text
.globl my_sum
my_sum:
    // n - edi
    // x - rsi
    mov eax, 0
    cmp edi, 0
start_loop:   
    jle return_eax
    add eax, DWORD PTR [rsi]
    add rsi, 4
    dec edi // and write compare with 0 flags
    
    jle return_eax
    sub eax, DWORD PTR [rsi]
    add rsi, 4
    dec edi // and write compare with 0 flags
    
    jmp start_loop
return_eax:
    ret
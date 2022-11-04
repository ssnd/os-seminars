    .intel_syntax noprefix
    .global syscall
    .text
syscall:
    // prologue
    push rbx
    push rbp
    push r12
    push r13
    push r14
    push r15
    // also keep r11 and rcx due to 'syscall' on x64 modifies them
    push r11
    push rcx
    // push arguments into memory to reaarange them
    push r9
    push r8
    push rcx
    push rdx
    push rsi
    push rdi
    // first argument is syscall number, move it into rax
    pop rax
    // move arguments into registers to match syscall calling conventions
    pop rdi
    pop rsi
    pop rdx
    pop r10
    pop r8
    // make actual syscall
    syscall
    // epilogue
    pop rcx
    pop r11
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    pop rbx
    ret
    
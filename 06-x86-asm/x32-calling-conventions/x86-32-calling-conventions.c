#include <stdio.h>


/***
 * This is the calling convention used for Win32, 
 * with exceptions for variadic functions 
 * (which necessarily use __cdecl) and 
 * a very few functions that use __fastcall. 
 * Parameters are pushed from right to left and the callee cleans the stack.
***/
void __attribute__((stdcall))  stdcall_function(int arg1, int arg2) {
    printf("%d\n", arg1 + arg2);
}

/***
 * The first two parameters are passed in ECX and EDX,
 * with the remainder passed on the stack as in __stdcall. 
 * Again, the callee cleans the stack. 
***/
void __attribute__((fastcall))  fastcall_function(int arg1, int arg2) {
    printf("%d\n", arg1 + arg2);
}

/***
 * The parameters are pushed from right to left 
 * (so that the first parameter is nearest to top-of-stack), 
 * and the caller cleans the parameters. 
 * Function names are decorated by a leading underscore.
***/
void __attribute__((cdecl))  cdecl_function(int arg1, int arg2) {
    printf("%d\n", arg1 + arg2);
}


int main() {
    stdcall_function(1,2);
    fastcall_function(1,2);
    cdecl_function(1,2);
}

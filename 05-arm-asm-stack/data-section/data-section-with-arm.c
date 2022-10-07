#include <stdio.h>

extern int asm_defined;

int c_defined = 15;

int func(); 

int main() {
    printf("c_defined=%d\n", c_defined);
    printf("func=%d\n", func());
    printf("asm_defined=%d\n", asm_defined);
    printf("c_defined=%d\n", c_defined);
    return 0;
}


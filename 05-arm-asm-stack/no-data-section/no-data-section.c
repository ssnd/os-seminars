#include <stdio.h>

static const int a = 2;
extern int asm_defined;
int test();

int main() {
    printf("%d\n", a);
    printf("%d\n", test());
    printf("%d\n", asm_defined);

    // asm_defined = 16; // ?
    printf("%d\n", test());
}
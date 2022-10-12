#include <stdio.h>
#include <stdint.h>

uint32_t func();

int main() {
    uint32_t  a = func();

    printf("a=%d\n",a );
    return 0;
}
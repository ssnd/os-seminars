#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main() {
    void *c1 = sbrk(0);
    printf("program break=%p\n", c1);
    c1 = (void*)((char*)c1+1);
    printf("c1=%p\n", c1);
    brk(c1);
    void * c2 = sbrk(0);
    printf("new program break=%p\n", c2);
    return 0;
}
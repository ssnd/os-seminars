#include <stdio.h>
#include <stdint.h>

int func(int64_t *);

int main() {
    int64_t a = 10;
    printf("%d\n", func(&a));
}
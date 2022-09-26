#include <stdio.h>
#include <stdint.h>

int main() {
    // const int32_t minus_five = -5;
    // const int32_t sixteen = 16;
    // const int32_t zero = 0;
    // const int32_t minus_one = -1;
    // printf("-5=0x%x, 16=0x%x, 0=0x%x, -1=0x%x\n", minus_five, sixteen, zero, minus_one);

    int result;
    int a = 12;
    int b = INT32_MAX;
    printf("%d\n", __builtin_sadd_overflow(a, b, &result));
    printf("result=%d\n", result);

   return 0;
}
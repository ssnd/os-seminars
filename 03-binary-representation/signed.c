#include <stdio.h>
#include <stdint.h>

int check_increment_signed(int32_t x) {
    return x + 1 > x;
}


int main() {

    printf("0x%x\n", -5);
    printf("check_increment_signed(INT32_MAX)=%d, check_increment_signed(0)=%d\n", 
            check_increment_signed(INT32_MAX), check_increment_signed(0));
}
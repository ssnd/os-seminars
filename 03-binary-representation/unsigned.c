#include <stdio.h>
#include <stdint.h>


int check_increment_unsigned(uint32_t x) {
    return x+1 > x;
}


int main() {
    const uint32_t two = 2;
    const uint32_t fifteen = 15;
    const uint32_t very_big_value = 100500;

    printf("2=0x%x, 15=0x%x, 100500=0x%x\n", two, fifteen, very_big_value);
    printf("UINT32_MAX=0x%x, UINT32_MAX+1=0x%x\n", UINT32_MAX, UINT32_MAX+1);
    printf("check_increment_unsigned(UINT32_MAX)=%d, check_increment_unsigned(0)=%d\n",
             check_increment_unsigned(UINT32_MAX), check_increment_unsigned(0));
    
    return 0;
}
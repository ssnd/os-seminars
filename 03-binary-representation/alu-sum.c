#include <stdio.h>
#include <stdint.h>
#include <limits.h>

typedef uint8_t byte;
static inline ITYPE bitSum(byte a, byte b, int* carry) {
    int old_carry = *carry;
    *carry = (a&b) | ((a^b) & (*carry));
    return a^b^old_carry;
}

void sum(ITYPE first, ITYPE second, ITYPE *res, int* CF) {
    *res = 0;
    size_t len_bits = sizeof(ITYPE) * CHAR_BIT;

    for (size_t i = 0; i < len_bits; ++i) {
        byte first_bit = first & 1;
        byte second_bit = second & 1;

        first >>=1; second >>=1;

        *res |= bitSum(first_bit, second_bit, CF) << i;
    }
}
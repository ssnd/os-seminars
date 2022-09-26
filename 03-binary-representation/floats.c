#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

typedef union {
    double    double_value;
    uint64_t  uint_value;
} real_bits;


void print_float_repr(double x) {
    real_bits v;
    v.double_value = x;

    printf("===\n%f\n", x);

    const size_t s = sizeof(uint64_t)*CHAR_BIT;
    // printf("%llu\n", x);
    for (int i = 63; i >= 0; --i) {
        if (i==63-1 || i==63-12) {
            printf(" ");
        }

        printf("%llx", ((1LL << i) & v.uint_value) >> i);
    }

    printf("\n===\n");
}


int main() {
    printf("basic examples\n");
    print_float_repr(1);
    print_float_repr(-1);
    print_float_repr(-3.1415);
    print_float_repr(3.1415);
    print_float_repr(100500);
    print_float_repr(-42.15);

    printf("powers of two\n");
    print_float_repr(0.5);
    print_float_repr(1);
    print_float_repr(2);
    print_float_repr(4);

    printf("very small numbers\n");
    print_float_repr(-1. / pow(2, 1023));
    print_float_repr(-1. / pow(2, 1023) / 2);

    return 0;
}
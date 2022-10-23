#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void vadd(float*, float*, float *);

int main() {
    float a[] = {1.,2.,3.,4.,5.,6.,7.,8.};
    float b[] = {2.,3.,4.,5.,6.,7.,8.,9.};

    float* c = calloc(8, sizeof(float));

    vadd(a,b,c);

    for (size_t i= 0; i < 8; ++i ){
        printf("c[%ld]=%f\n", i, c[i]);
    }

    return 0;
}
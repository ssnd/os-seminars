#include <stdio.h>
#include <immintrin.h>

void vadd() { 
    __m256 a = _mm256_set_ps(1.,2.,3.,4.,5.,6.,7.,8.);
    __m256 b = _mm256_set_ps(1.,2.,3.,4.,5.,6.,7.,8.);

    a = _mm256_add_ps(a, b);

    for (size_t i= 0; i < 8; ++i ){
        printf("c[%ld]=%f\n", i, a[i]);
    }
}


void vadd_with_load() { 
    float a[] = {1.,2.,3.,4.,5.,6.,7.,8.};
    float b[] = {1.,2.,3.,4.,5.,6.,7.,8.};
    __m256 a_2 = _mm256_loadu_ps(a);
    __m256 b_2 = _mm256_loadu_ps(b);

    a_2 = _mm256_add_ps(a_2, b_2);

    for (size_t i= 0; i < 8; ++i ){
        printf("c[%ld]=%f\n", i, a_2[i]);
    }
}

int main() {
    vadd_with_load();
}

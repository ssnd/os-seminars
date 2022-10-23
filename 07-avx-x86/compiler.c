#include <immintrin.h>
#include <stdio.h>

void sum_0(const int* a, const int * b, int *c) {
    for (size_t i =0; i < 4; ++i ){
        c[i] = a[i] + b[i];
    }
}

void sum_1(const int * restrict a, const int * restrict b, int * restrict c) {
    for (size_t i = 0; i < 4; ++i ){
        c[i] = a[i] + b[i];
    }
}

void sum_2(const int* restrict a_, const int* restrict b_, int* restrict c_) {
    const int * a = __builtin_assume_aligned(a_, 16);
    const int * b = __builtin_assume_aligned(b_, 16);
    int * c = __builtin_assume_aligned(c_, 16);

    for (size_t i =0; i < 4; ++i ){
        c[i] = a[i] + b[i];
    }
}

void sum_3(const int * a, const int * b, const int * c) {
    (*(__m128i*)c) = _mm_add_epi32((*(__m128i*)a), (*(__m128i*)b));
}


/*
int main () {
    int shift = 13; // if shift=1 the world crumbles apart

    int * a = aligned_alloc(32, (4+shift)*sizeof(int));
    int * b = aligned_alloc(32, (4+shift)*sizeof(int));
    int * c = aligned_alloc(32, (4+shift)*sizeof(int));

    a = (int*)((void* )a+1);
    b = (int*)((void* )b+1);
    c = (int*)((void* )c+1);

    printf("%p\n", a);

    shift =0 ;
    a[shift]=1; a[1+shift]=2; a[2+shift]=3; a[3+shift]=4;
    b[shift]=1; b[1+shift]=2; b[2+shift]=3; b[3+shift]=4;
    sum_3(a+shift, b+shift, c+shift);

    printf("%d %d %d %d\n", c[shift], c[1+shift], c[2+shift],c[3+shift]);
}
*/
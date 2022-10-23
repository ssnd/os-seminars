#include <stdio.h>
#include <stdlib.h>

double very_important_function(size_t N, const float *A, const float *B, float *R);

void fill_values(float * a, const size_t n) {
    for (size_t i = 0; i < n; ++i) {
        a[i] = (float) i+1;
    }
}

void display_values(float * a, const size_t n) {
    for (size_t  i = 0; i < n; ++i) {
        printf("%f ", a[i]);
    }

    printf("\n");
}

int main() {
    const size_t N = 8;
    float * A = aligned_alloc(32, N*sizeof(float));
    float * B = aligned_alloc(32, N*sizeof(float));
    float * R = aligned_alloc(32, N*sizeof(float));

    fill_values(A, N);
    fill_values(B, N);

    printf("%f\n", very_important_function(N, A, B, R));

    display_values(R, N);

    return 0;
}
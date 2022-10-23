#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void very_important_function(size_t N);

int *A;

int main() {
    A = malloc(sizeof(int)*7);
    A[0] = 2;
    A[1] = 1;
    A[2] = 2;
    A[3] = 3;
    A[4] = 4;
    A[5] = 5;
    A[6] = 6;
    very_important_function(7);

    free(A);
}
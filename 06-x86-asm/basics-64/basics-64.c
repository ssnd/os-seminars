#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void very_important_function(size_t N);

int *A;

int main() {
    A = malloc(sizeof(int)*5);
    A[0] = 1;
    A[1] = 1;
    A[2] = 1;
    A[3] = 1;
    A[4] = 1;
    very_important_function(5);

    free(A);
}
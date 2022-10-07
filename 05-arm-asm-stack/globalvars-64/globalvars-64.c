#include <stdio.h>
#include <stdint.h>

uint64_t A = 1;
uint64_t B = 1;
uint64_t C = 1;
uint64_t D = 1;

extern uint64_t R;

void calculate();

int main() {
    calculate();
    printf("R=%lu\n", R);
}
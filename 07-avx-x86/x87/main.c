#include <stdio.h>

double square(double x) {
    return x*x;
}

int main() {
    printf("10*10=%f\n", square(10.f));
    return 0;
}
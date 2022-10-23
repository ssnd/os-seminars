#include <stdio.h>

double add(double a, double b) {
    return a + b;
}

double mult(double a, double b) {
    return a * b;
}

int cmp(double a) {
    return a > 0 ? 42 : 0;
}

double max(double a, double b) {
    return a > b ? a : b;
}

double muldi(double a, int b) {
    return a * b;
}
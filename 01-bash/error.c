#include <stdio.h>

int main() {
    fprintf(stdout, "INFO hello, this is an important message\n");
    fprintf(stderr, "ERROR hello, something very bad happened\n");
    return 0;
}
#include <stdio.h>
#include <unistd.h>

// disclaimer: this is a simple example meant to demonstrate what fflush does
int main() {
    printf("hello world");
    fork();
}

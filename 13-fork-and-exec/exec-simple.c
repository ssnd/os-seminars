#include <stdio.h>
#include <unistd.h>

int main() {
    printf("before exec, pid=%d\n", getpid());
    execlp("python", "python", "-c", "import os; print(f\"hello, pid={os.getpid()}\")", NULL);

    // not coming back unless something bad happens
    perror("something bad happened");
    return 1;
}
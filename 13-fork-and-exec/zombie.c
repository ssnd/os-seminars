#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (-1 == pid) {
        perror("fork");
        exit(1);
    } else if (0 == pid) {
        printf("child process mypid=%d\n", getpid());
        exit(0);
    } else {
        printf("parent process, mypid=%d, childpid=%d\n", getpid(), pid);
        sleep(60);
    }
}
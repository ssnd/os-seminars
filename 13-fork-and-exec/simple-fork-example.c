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
        exit(1);
    } else {
        printf("parent process, mypid=%d, childpid=%d\n", getpid(), pid);
        int status;
        pid_t w = waitpid(pid, &status, 0);
        if (-1==w) {
            perror("waitpid");
            exit(1);
        }

        printf("child exit status %d\n", WEXITSTATUS(status));
    }

}
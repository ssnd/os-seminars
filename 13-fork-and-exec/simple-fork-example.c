#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork(); // the program "splits" in two from now on

    if (-1 == pid) { // make sure fork() executed without an error
        perror("fork");
        exit(1);
    } else if (0 == pid) { // child will execute instructions from this branch
        printf("child process mypid=%d\n", getpid());
        exit(1);
    } else { // parent process will know the child's pid from the value returned from fork()
        printf("parent process, mypid=%d, childpid=%d\n", getpid(), pid);
        int status;
        pid_t w = waitpid(pid, &status, 0); // wait until the child is finished
        if (-1==w) {
            perror("waitpid");
            exit(1);
        }

        printf("child exit status %d\n", WEXITSTATUS(status)); // print out the child's exit code
    }

}
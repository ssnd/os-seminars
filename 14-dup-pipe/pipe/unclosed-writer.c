#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const int READ = 0;
static const int WRITE =1;

static const char mymessage[] = "hello world\n";

int main() {
    int pipefd[2];

    if (-1 == pipe(pipefd)) {
        perror("pipe");
        return 1;
    }

    int childpid = fork();
    if (-1 == childpid) {
        perror("fork");
        return 1;
    }

    if (childpid == 0) { // child
        close(pipefd[READ]); // close the read end of the pipe

        printf("child: sending message\n");
        write(pipefd[WRITE], mymessage, strlen(mymessage)); // write the message
        printf("child: message sent\n");

        close(pipefd[WRITE]); // close the write end when we're done
        return 0;
    } else { // parent
        // Without the pipefd[WRITE] fd closed, the program will deadlock.
        // The child process holds a copy of the parent fds after fork() is called.
        // Without ALL fds closed, the child's read syscall will not receive EOF and wait forever.
        close(pipefd[WRITE]); // TODO: try removing this line 

        printf("parent: reading the message\n");
        char ch;
        while (read(pipefd[READ], &ch, 1) > 0) {
            write(STDOUT_FILENO, "recv: ", 6);
            write(STDOUT_FILENO, &ch, 1);
            write(STDOUT_FILENO, "\n",1);
            fflush(stdout);
        }
        printf("parent: the message was received\n");

        close(pipefd[READ]); // close the fd when done
        wait(NULL); 
        return 0;
    }


}
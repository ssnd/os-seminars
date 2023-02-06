#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

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
        uint64_t counter = 0;
        uint64_t buff_size = 0;
        while (1) {
            if (++counter % 100 == 0) {
                printf("child: still writing, buf_size=%llu\n", buff_size);
            }
            write(pipefd[WRITE], mymessage, strlen(mymessage)); // write the message
            buff_size += strlen(mymessage);

        }
        printf("child: message sent\n");

        close(pipefd[WRITE]); // close the write end when we're done
        return 0;
    } else { // parent
        close(pipefd[WRITE]); // close the write end of the pipe

        printf("parent: not doing anything\n");

        while (1) {
            sched_yield();
        }
        close(pipefd[READ]);
        wait(NULL); 
        return 0;
    }


}
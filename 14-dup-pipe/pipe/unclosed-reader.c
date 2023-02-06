#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sched.h>

static const int READ = 0;
static const int WRITE =1;

void sigpipe_handler(int signum){
    printf("\nSIGPIPE received, closing...\n");
    exit(1);
}

static const char mymessage[] = "hello world\n";

int main() {
    // If a pipe is written to without any readers, the writing process receives a SIGPIPE signal.
    // A custom signal handler is called when the process receives a SIGPIPE signal.
    signal(SIGPIPE, sigpipe_handler); // Register SIGPIPE handler

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
        // Without closing the reader, the child process is not going to know 
        // about the closed pipe read end in the parrent process and will not receive the SIGPIPE. 
        // That's bad, because it leaves us with another deadlock!
        close(pipefd[READ]); // TODO: try removing this line  

        // Unlike pipe-unclosed-writer.c, this program write characters one by one, forever
        printf("child:  sending messages\n");
        while (1) {
            for (size_t i = 0; i < strlen(mymessage); ++i) {
                printf("child:  send -> %c\n", mymessage[i]);
                write(pipefd[WRITE], mymessage+i, 1); // write the message
                sleep(1);
            }
        }

        // This is never going to be called
        printf("child:  all messages sent\n");
        close(pipefd[WRITE]); // close the write end when we're done

        return 0;
    } else { // parent
        close(pipefd[WRITE]);  

        printf("parent: reading messages\n");
        char ch;
        int counter = 5;

        // The parent only reads five characters though, and then closes.
        while (read(pipefd[READ], &ch, 1) > 0 && --counter > 0) {
            printf("parent: recv <- %c\n", ch);
        }
        printf("parent: я усталь\n");

        close(pipefd[READ]);
        wait(NULL);
        return 0;
    }

}
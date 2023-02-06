#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int read_fd;
    int write_fd;
} MyPipe;

static const char mymessage[] = "hello world\n";

int main() {
    MyPipe p;

    if (-1 == pipe((int*)&p)) {
        perror("pipe");
        return 1;
    }

    int childpid = fork();
    if (-1 == childpid) {
        perror("fork");
        return 1;
    }

    if (childpid == 0) { // child
        close(p.read_fd); // close the read end of the pipe

        printf("child: sending message\n");
        write(p.write_fd, mymessage, strlen(mymessage)); // write the message
        printf("child: message sent\n");

        close(p.write_fd); // close the write end when we're done
        return 0;
    } else { // parent
        close(p.write_fd); // close the write end of the pipe

        printf("parent: reading the message\n");
        char ch;
        while (read(p.read_fd, &ch, 1) > 0) {
            write(STDOUT_FILENO, "recv: ", 6);
            write(STDOUT_FILENO, &ch, 1);
            write(STDOUT_FILENO, "\n",1);
            fflush(stdout);
        }
        printf("parent: the message was received\n");

        close(p.read_fd);
        wait(NULL); 
        return 0;
    }


}
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int read_fd;
    int write_fd;
} MyPipe ;

static const int BUF_SIZE = 4096 * 2;


int child_fn(const int proc_no, MyPipe* p) {
    char buf[BUF_SIZE];
    memset(buf, '0' + proc_no, BUF_SIZE);

    close(p->read_fd); // close the read end of the pipe
    while (1) {
        write(p->write_fd, buf, BUF_SIZE); // write the message
    }

    close(p->write_fd); // close the write end when we're done
    return 0;
}

int parent_fn(MyPipe* p) {
    close(p->write_fd); // close the write end of the pipe

    printf("parent: reading the message\n");
    char ch[BUF_SIZE];
    while (read(p->read_fd, ch, BUF_SIZE) > 0) {
        // write(STDOUT_FILENO, "recv: ", 6);
        write(STDOUT_FILENO, ch, BUF_SIZE);
        fflush(stdout);
    }
    printf("parent: the message was received\n");

    close(p->read_fd);
    wait(NULL); 
    return 0;
}

int main() {
    MyPipe p;

    if (-1 == pipe((int*)&p)) {
        perror("pipe");
        return 1;
    }

    int child1 = fork(); // first fork
    if (-1 == child1) {
        perror("fork");
        return 1;
    } else if (child1 == 0) {
        return child_fn(1, &p);
    }

    int child2 = fork(); // second fork
    if (-1 == child2) {
        perror("fork");
        return 1;
    } else if (child2 == 0) {
        return child_fn(2, &p);
    }

    return parent_fn(&p);

}
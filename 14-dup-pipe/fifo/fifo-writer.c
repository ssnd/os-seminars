#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

static const char fifo_path[]  = "/tmp/fifo1";

int main() {
    if (-1 == mkfifo(fifo_path, 0644) && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    int fd = open(fifo_path, O_WRONLY, 0644);
    if (-1 == fd) {
        perror("open");
        return 1;
    }

    printf("press enter\n");
    fgetc(stdin);

    for (size_t  i = 0 ; i < 10; ++i) {
        printf("writing to fifo1\n");
        write(fd, "123", 3);
        sleep(1);
    }

    close(fd);

    printf("done\n");

    return 0;
}
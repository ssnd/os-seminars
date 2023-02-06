#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static const char fifo_path[]  = "/tmp/fifo1";

int main() {
    int fd = open(fifo_path, O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return 1;
    }

    
    while (1) {
        char ch;
        read(fd, &ch,1);
        printf("recv: %c\n", ch);
        sleep(1);
    }

    close(fd);

    return 0;
}